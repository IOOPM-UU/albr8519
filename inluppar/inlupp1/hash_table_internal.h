#pragma once
#include "hash_table.h"

#define No_Buckets 17

typedef struct entry entry_t;

struct entry
{
  elem_t key;      // holds the key
  elem_t value;      // holds the value
  entry_t *next;  // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[No_Buckets];
  ioopm_hash_function *hash_fn;
  ioopm_eq_function *key_eq_fn;
};