#include "hash_table.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct entry entry_t;

struct entry
{
    char *key;      // holds the key
    int value;      // holds the value
    entry_t *next;  // points to the next entry (possibly NULL)
};

struct hash_table
{
    entry_t buckets[17];
};

ioopm_hash_table_t *ioopm_hash_table_create(void) {
    // Allocate space for a ioopm_hash_table_t = 17 pointers to entry_t's.
    return calloc(1, sizeof(ioopm_hash_table_t));
}

static void entry_destroy(entry_t *entry) 
{
  free(entry);
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < 17; ++i)
  {
    entry_t *current = ht->buckets[i].next;   // hoppa över sentinel-noden
    while (current != NULL)
    {
      entry_t *next = current->next;
      entry_destroy(current);
      current = next;
    }
  }
  free(ht);
}

 // Static: Only visible in this file
static size_t string_knr_hash(const char *str)
{
  size_t result = 0;
  while (*str != '\0')
  {
    result = result * 31 + ((unsigned char) *str);
    str++;
  }
  return result;
}

static entry_t *entry_create(char *key, int value, entry_t *next)
{
  entry_t *entry = calloc(1, sizeof(entry_t));
  entry->key = key;
  entry->value = value;
  entry->next = next;
  return entry;
}

static entry_t *find_previous_entry(ioopm_hash_table_t *ht, char *key)
{
  size_t bucket = string_knr_hash(key) % 17;
  entry_t *previous = &ht->buckets[bucket];   // börja i sentinel-noden

  while (previous->next != NULL && strcmp(previous->next->key, key) != 0)
  {
    previous = previous->next;
  }
  return previous;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, char *key, int value)
{
  // find previous entry, or the last entry if the key does not exist
  entry_t *previous = find_previous_entry(ht, key);

  // if the key exists, update the value, otherwise create a new entry
  if (previous->next != NULL)
  {
    previous->next->value = value;
  }
  else
  {
    previous->next = entry_create(key, value, NULL);
  }
}

bool ioopm_hash_table_lookup(ioopm_hash_table_t *ht, char *key, int *result)
{
  entry_t *previous = find_previous_entry(ht, key);
  if (previous->next != NULL)
  {
    *result = previous->next->value;
    return true;
  }
  return false;
}