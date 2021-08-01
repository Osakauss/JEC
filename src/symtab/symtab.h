#ifndef JEC_SYMTAB_H
#define JEC_SYMTAB_H

/**
 *  apropos:
 * 
 * the symtab is supposed to make working with strings easier
 * and more performant
 * 
 * it works by managing a bunch of buckets containing strings,
 * addressable by plain integers
 * 
 * it allows us to make tokens that are very cheap, and giving
 * us all the control over the generated strings (maybe even
 * other data later on)
 */

typedef unsigned long sym_t;
typedef struct symtab_bucket_t symtab_bucket_t;

typedef struct symtab_t {
    symtab_bucket_t *buckets;
} symtab_t;

symtab_t *
symtab(void);

void
symtab_free(symtab_t *tab);

/**
 * insert a string to the table and obtain the reference through
 * `sym`
 * 
 * optionally specify `owned` as true for less data duplication
 * when the table can take _full_ ownership of the string (including free)
 */
int
symtab_insert(symtab_t *tab, sym_t *sym, const char *s, int owned);

/**
 * through a previously obtained `sym`, get a const reference to the
 * corresponding string
 */
const char *
symtab_query(const symtab_t *tab, sym_t sym);

#endif
