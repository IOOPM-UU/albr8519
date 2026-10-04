#include "hash_table_iterator.h"
#include "hash_table_internal.h"
#include <stdlib.h>
#include <assert.h>

struct hash_table_iterator
{
  ioopm_hash_table_t *ht;
  int current_bucket;
  entry_t *current_entry;
};

static void advance_iterator_state(ioopm_hash_table_iterator_t *it)
{
  it->current_entry = it->current_entry->next;

  if (it->current_entry == NULL)
  {
    it->current_bucket += 1;

    if (it->current_bucket != No_Buckets)
    {
      it->current_entry = &it->ht->buckets[it->current_bucket];
    }
  }
}

static void skip_sentinel_nodes(ioopm_hash_table_iterator_t *it)
{
  while (it->current_bucket != No_Buckets &&
         it->current_entry == &it->ht->buckets[it->current_bucket])
  {
    advance_iterator_state(it); // Cheat!
  }
}

ioopm_hash_table_iterator_t *ioopm_hash_table_iterator_create(ioopm_hash_table_t *ht)
{
    ioopm_hash_table_iterator_t *it = malloc(sizeof(ioopm_hash_table_iterator_t));

    it->ht = ht;
    it->current_bucket = 0;
    it->current_entry = &ht->buckets[0];
    skip_sentinel_nodes(it); 
    return it;
}

void ioopm_hash_table_iterator_destroy(ioopm_hash_table_iterator_t *it)
{
    free(it);
}

bool ioopm_hash_table_iterator_at_end(ioopm_hash_table_iterator_t *it)
{
  return it->current_bucket == No_Buckets;
}

void ioopm_hash_table_iterator_advance(ioopm_hash_table_iterator_t *it)
{
    assert(!ioopm_hash_table_iterator_at_end(it) && "Iterator at end when trying to advance!");
    advance_iterator_state(it);
    skip_sentinel_nodes(it);
}

char* ioopm_hash_table_iterator_current_key(ioopm_hash_table_iterator_t *it)
{
  assert(!ioopm_hash_table_iterator_at_end(it) && "iterator at end when reading key");
  return it->current_entry->key;
}

int ioopm_hash_table_iterator_current_value(ioopm_hash_table_iterator_t *it)
{
  assert(!ioopm_hash_table_iterator_at_end(it) && "iterator at end when reading value");
  return it->current_entry->value;
}