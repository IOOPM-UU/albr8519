#pragma once
#include <stdbool.h>
#include <stddef.h>
#include "common.h"

/**
* @file hash_table.h
* @author Alexander Brundin
* @date 1st October 2026
* @brief A generic hash table mapping elem_t keys to elem_t values.
*
* Keys are hashed and compared using functions(hash_fn & key_eq_fn) that are supplied by the user when the table is created.
* If a key or value points to allocated memory, the user is responsible for freeing it, after the table has been destroyed or an entry removed 
*
* The table uses a fixed number of buckets, 17. This can be changed in the hash_tavl_internal.h file
*/
typedef struct hash_table ioopm_hash_table_t;

/// @brief Creates a new empty hash table
/// @param hash_fn function that computes the hash of a key
/// @param key_eq_fn function that compares two keys to see if they're equal
/// @return a new hash table
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function *hash_fn, ioopm_eq_function *key_eq_fn);

/// @brief Delete a hash table, keys and values are not freed
/// @param ht hash table to be deleted
void ioopm_hash_table_destroy(ioopm_hash_table_t *ht);

/// @brief adds a key => value mapping. If key exists, its value is replaced. Previously stored key is kept
/// @param ht hash table to insert to
/// @param key key to insert or to map to the value
/// @param value value to be inserted
void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value);

/// @brief lookup value for key in hash table ht
/// @param ht hashtable to be operated upon
/// @param key key to lookup
/// @param result where the value is written if the key exists, unchanged if not
/// @return true if the key exists, false if not. 
bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief remove any mapping from key to a value
/// @param ht hash table
/// @param key key to remove
/// @param result pointer to where the removed value will be written, if the key existed, unchanged if not
/// @return true if the key existed and was removed, false otherwise
bool ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key, elem_t *result);

/// @brief checks for a key in a hash table
/// @param ht hashtable to operate upon
/// @param key key to search for
/// @return true if key is found, false if not
bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key);

/// @brief checks that the size of a hash table is equal to 0
/// @param ht hash table to operate upon
/// @return true if hash table is empty, false if not
bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht);

/// @brief checks the size (number of entries) in a hash table
/// @param ht hash table to operate upon
/// @return the size of the hash table
size_t ioopm_hash_table_size(ioopm_hash_table_t *ht);