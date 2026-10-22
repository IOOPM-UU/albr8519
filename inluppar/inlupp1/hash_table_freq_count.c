#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hash_table.h"
#include "hash_table_iterator.h"
#include "common.h"

/// NOTE: from copy_freq_count.c provided by instructions ///
#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

// A word and the number of times it occurs
/// @brief A word together with its frequency
typedef struct
{
  char *word;
  int count;
} word_count_t;
/// NOTE: END

/// @brief Process a single word, updating its frequency
/// @param ht a hash table containing the frequencies of the words found so far
/// @param word the word to process
static void process_word(char *word, ioopm_hash_table_t *ht)
{
  elem_t freq;
  if (ioopm_hash_table_lookup(ht, string_elem(word), &freq))
  {
    ioopm_hash_table_insert(ht, string_elem(word), int_elem(freq.i + 1));   // existing key is kept
  }
  else
  {
    ioopm_hash_table_insert(ht, string_elem(strdup(word)), int_elem(1));  // new word: store a copy
  }
}

/// @brief Process a single file, updating the frequencies of its words
/// @param filename the name of the file to process
/// @param ht a hash table containing the frequencies of the words found so far
void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");
  if (f == NULL)
  {
    fprintf(stderr, "Could not open %s\n", filename);
    return;
  }

  char *buf = NULL;
  size_t len = 0;
  while (getline(&buf, &len, f) != -1)
  {
    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      process_word(word, ht);
    }
  }
  free(buf);
  fclose(f);
}

// Sort by count, highest first
static int compare_counts(const void *a, const void *b)
{
  const word_count_t *wa = a;
  const word_count_t *wb = b;
  return wb->count - wa->count;
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s file1 ... fileN\n", argv[0]);
    return 1;
  }

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  for (int index = 1; index < argc; index++)
  {
    process_file(argv[index], ht);
  }

  size_t size = ioopm_hash_table_size(ht);
  word_count_t *words = calloc(size, sizeof(word_count_t));

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  for (size_t i = 0; !ioopm_hash_table_iterator_at_end(it); ++i)
  {
    words[i].word = ioopm_hash_table_iterator_current_key(it).s;
    words[i].count = ioopm_hash_table_iterator_current_value(it).i;
    ioopm_hash_table_iterator_advance(it);
  }
  ioopm_hash_table_iterator_destroy(it);

  qsort(words, size, sizeof(word_count_t), compare_counts);
  for (size_t i = 0; i < size; ++i)
  {
    printf("%s: %d\n", words[i].word, words[i].count);
  }

  for (size_t i = 0; i < size; ++i)
  {
    free(words[i].word);
  }
  free(words);
  ioopm_hash_table_destroy(ht);

  return 0;
}