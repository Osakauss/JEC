#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "../jec/defs.h"

/**
 *  item indexing in the table:
 * 
 * we have some buckets, i.e. 512 or 1<<9,
 * this means we need 9 bits for indexing the bucket,
 * the rest of the bits represent the in-bucket index
 */

// ===== DEFINES =====

// bits for the power of the bucket size
#define SYMTAB_BUCKETS_BITS         (9)
// amount of buckets for a single symtab
#define SYMTAB_BUCKETS              (1<<SYMTAB_BUCKETS_BITS)
// initial allocated space of a single bucket
#define BUCKET_SIZE                 (64)
// amount of added bucket space per reallocation step
#define BUCKET_REALLOC_SIZE         (64)

// ===== STRUCTS =====

/**
 * a bucket is simply a dynamic array of char pointers
 * 
 * by adding strings to the bucket, it returns their index for
 * future accessing
 * 
 * limitations: the bucket can only grow, otherwise previous
 * indices become obsolete
 */

typedef struct symtab_bucket_t {
    size_t size;
    size_t cap;
    char **values;
} symtab_bucket_t;

static int
bucket_setup(symtab_bucket_t *buc)
{
    buc->values = malloc(BUCKET_SIZE * sizeof(*buc->values));
    if (!buc->values)
        return FAILURE;
    
    buc->size = 0;
    buc->cap = BUCKET_SIZE;
    return SUCCESS;
}

static void
bucket_teardown(symtab_bucket_t *buc)
{
    for (size_t i = 0; i < buc->size; i++) {
        free(buc->values[i]);
    }

    free(buc->values);
}

static int
bucket_realloc(symtab_bucket_t *buc)
{
    char **old;
    size_t oldsize, newsize;

    oldsize = buc->size + buc->cap;
    newsize = oldsize + BUCKET_REALLOC_SIZE;

    old = buc->values;
    buc->values = realloc(buc->values, newsize);
    if (!buc->values) {
        buc->values = old;
        return FAILURE;
    } else {
        buc->cap += BUCKET_REALLOC_SIZE;
        return SUCCESS;
    }
}

/**
 * attempt to insert a possibly owned string, obtaining in `idx` the
 * reference of the item in the bucket
 */
static int
bucket_insert(symtab_bucket_t *buc, const char *s, size_t *idx, int owned)
{
    size_t i;

    // look for exisintg string
    for (i = 0; i < buc->size; i++) {
        int cmp = strcmp(s, buc->values[i]);
        if (cmp == 0) {
            *idx = i;
            if (owned) // explicit cast since we can claim ownership
                free((char*)s);
            return SUCCESS;
        }
    }

    // expand if necessary
    if (buc->cap == 0) {
        if(bucket_realloc(buc) == FAILURE) {
            return FAILURE;
        }
    }

    // get owned string if necessary
    char *owned_s;
    if (owned) {
        owned_s = (char*)s;
    } else {
        owned_s = strdup(s);
        if (!owned_s)
            return FAILURE;
    }

    // store
    i = buc->size;
    *idx = i;
    buc->values[i] = owned_s;
    buc->size++;
    buc->cap--;
    return SUCCESS;
}

/**
 * by giving the index for an item in this bucket, return a
 * reference to the value
 */
static int
bucket_query(symtab_bucket_t *buc, size_t idx, const char **s)
{
    size_t i;

    if (buc->size <= idx) {
        return FAILURE;
    }

    *s = buc->values[idx];
    return SUCCESS;
}

// ===== STATIC =====

/**
 * a basic hash function for strings
 */
#define HASH_P0 54059 /* a prime */
#define HASH_P1 76963 /* another prime */
#define HASH_FIRSTH 37 /* also prime */
static unsigned
hash_str(const char* s)
{
   unsigned h = HASH_FIRSTH;

   while (*s) {
     h = (h * HASH_P0) ^ (s[0] * HASH_P1);
     s++;
   }
   return h;
}

/**
 * from bucket index and in-bucket index, obtain complete sym_t
 */
static inline sym_t
sym_from_idxs(size_t b_idx, size_t idx)
{
    return b_idx + (idx << SYMTAB_BUCKETS_BITS);
}

/**
 * from sym_t get bucket and in-bucket indices
 */
static inline void
idxs_from_sym(sym_t sym, size_t *b_idx, size_t *idx)
{
    *idx = sym >> SYMTAB_BUCKETS_BITS;
    *b_idx = sym & ((1 << SYMTAB_BUCKETS_BITS) - 1);
}

// ===== EXPORTS =====

symtab_t *
symtab(void)
{
    symtab_t *tab;

    tab = malloc(sizeof (*tab));
    if (!tab)
        return NULL;

    tab->buckets = malloc(sizeof (*tab->buckets) * SYMTAB_BUCKETS);
    if (!tab->buckets) {
        free(tab);
        return NULL;
    }

    for (size_t b = 0; b  < SYMTAB_BUCKETS; b++) {
        bucket_setup(&tab->buckets[b]);
    }

    return tab;
}

void
symtab_free(symtab_t *tab)
{
    for (size_t b = 0; b < SYMTAB_BUCKETS; b++) {
        bucket_teardown(&tab->buckets[b]);
    }

    free(tab->buckets);
    free(tab);
}

int
symtab_insert(symtab_t *tab, sym_t *sym, const char *s, int owned)
{
    size_t b_idx;
    size_t idx;
    
    b_idx = hash_str(s) % SYMTAB_BUCKETS;
    
    if (bucket_insert(&tab->buckets[b_idx], s, &idx, owned) == SUCCESS) {
        if (sym)
            *sym = sym_from_idxs(b_idx, idx);
        return SUCCESS;
    } else {
        return FAILURE;
    }
}

const char *
symtab_query(const symtab_t *tab, sym_t sym)
{
    const char *s;
    size_t b_idx, idx;

    idxs_from_sym(sym, &b_idx, &idx);

    if (bucket_query(&tab->buckets[b_idx], idx, &s) == SUCCESS) {
        return s;
    } else {
        return NULL;
    }
}
