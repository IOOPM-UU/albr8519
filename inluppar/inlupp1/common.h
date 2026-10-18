#pragma once
#include <stddef.h>
#include <stdbool.h>

/**
 * @file common.h
 * @author Alexander Brundin
 * @date 1 October 2026
 * @brief Generic element type and helper functions shared by the
 * hash table and the linked list.
 */

#define int_elem(x)   ((elem_t) { .i = (x) })
#define bool_elem(x)  ((elem_t) { .b = (x) })
#define ptr_elem(x)   ((elem_t) { .p = (x) })
#define string_elem(x) ((elem_t) { .s = (x) })


typedef union elem elem_t;
/// @brief A function that decides whether two elements are equal
typedef bool ioopm_eq_function(elem_t a, elem_t b);
/// @brief A function that calculates the hash for a key
typedef size_t ioopm_hash_function(elem_t key);

/// @brief Hash function for string keys, computed from the characters of the string
/// @param key the key to hash, key.s must point to a valid, non-NULL string
/// @return a hash value that is equal for all strings with the same characters
/// NOTE: Use together with string_eq.
size_t string_hash(elem_t key);

/// @brief Function that compares the characters of two strings
/// @param a first key, a.s must point to a valid, non NULL string
/// @param b second key b.s must point to a valid, non NULL string
/// @return true if the strings contain the same characters, false if not
/// NOTE: Use together with string_hash.
bool string_eq(elem_t a, elem_t b);

/// @brief Hash function for int keys, computed from the size of the int
/// @param key the key to hash
/// @return the value of key.i
/// NOTE: Use together with int_eq.
size_t int_hash(elem_t key);

/// @brief Function that compares the values of a.i and b.i
/// @param a first key, a.i 
/// @param b second key, b.i 
/// @return true if a.i and b.i is equal, false otherwise
/// NOTE: Use together with int_hash.
bool int_eq(elem_t a, elem_t b);

/// @brief Hash function for pointer keys, the hash adress the pointer holds
/// @param key the key to hash, read key.p
/// @return the address key.p holds as size_t. Two keys only get the same
/// hashes if they are the same pointer
/// NOTE: Use together with pointer_eq.
size_t pointer_hash(elem_t key);

/// @brief Identity for pointers, compares the pointers
/// @param a first pointer, read a.p
/// @param b second pointer, read b.p
/// @return true if a.p and b.p is the same address, otherwise false
/// NOTE: Use together with pointer_hash.
bool pointer_eq(elem_t a, elem_t b);

union elem
{
    int i;              ///< Integer
    unsigned int u;     ///< Unsigned integer
    bool b;             ///< Boolean
    float f;            ///< Floating point number
    void *p;            ///< Pointer to arbitrary data
    char *s;            ///< String
};

