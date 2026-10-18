#include <string.h>
#include <stdint.h>
#include "common.h"

/// Hash a string key by walking it character by character
size_t string_hash(elem_t key)
{
    size_t result = 0;
    for(char *c = key.s; *c != '\0'; c++)
    {
        result = result * 31 + (unsigned char) *c;
    }
    return result;
}
/// @brief Equivalence for strings: equal if they contain the same characters
/// @param a 
/// @param b 
/// @return 
bool string_eq(elem_t a, elem_t b)
{
    return strcmp(a.s, b.s) == 0;
}

///
size_t int_hash(elem_t key)
{
    return (size_t) key.i;
}

bool int_eq(elem_t a, elem_t b)
{
    return a.i == b.i;
}

size_t pointer_hash(elem_t key)
{
  return (size_t) (uintptr_t) key.p;
}

bool pointer_eq(elem_t a, elem_t b)
{
    return a.p == b.p;
}