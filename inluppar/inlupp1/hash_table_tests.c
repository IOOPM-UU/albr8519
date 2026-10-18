#include "hash_table.h"
#include <CUnit/Basic.h>
#include "hash_table_iterator.h"
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "common.h"

/// NOTE: gcc -Wall -Wextra -g hash_table.c hash_table_iterator.c hash_table_tests.c -o hash_table_tests -lcunit && ./hash_table_tests
/// NOTE: valgrind --leak-check=full ./hash_table_tests

int init_suite(void) {
  // Change this function if you want to do something *before* you
  // run a test suite
  return 0;
}

int clean_suite(void) {
  // Change this function if you want to do something *after* you
  // run a test suite
  return 0;
}

void test_create_destroy(void)
{
   ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_once(void)
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  elem_t key = string_elem("abc");
  elem_t value = int_elem(123);

  // check that key is not in ht
  elem_t result = int_elem(0);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, value.i);

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

// Inserting a key that is already in the table overwrites its value instead of adding a duplicate entry
void test_insert_existing_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  elem_t key = string_elem("abc");

  ioopm_hash_table_insert(ht, key, int_elem(1));
  ioopm_hash_table_insert(ht, key, int_elem(2));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result.i, 2);

  ioopm_hash_table_destroy(ht);
}

// Two different keys can be stored at the same time, and lookup returns the correct value for each.
void test_insert_new_new(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("def"), int_elem(2));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));
  CU_ASSERT_EQUAL(result.i, 1);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("def"), &result));
  CU_ASSERT_EQUAL(result.i, 2);

  ioopm_hash_table_destroy(ht);
}

// Inserting a new key after an overwrite does not affect the overwritten entry.
void test_insert_existing_new(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(2));   // 2: existing
  ioopm_hash_table_insert(ht, string_elem("def"), int_elem(3));   // 1: new

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));
  CU_ASSERT_EQUAL(result.i, 2);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("def"), &result));
  CU_ASSERT_EQUAL(result.i, 3);

  ioopm_hash_table_destroy(ht);
}

// When the same key is overwritten several times, lookup always returns the most recent value.
void test_insert_existing_existing(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(2));   // 2: befintlig
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(3));   // 2: befintlig

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));
  CU_ASSERT_EQUAL(result.i, 3);

  ioopm_hash_table_destroy(ht);
}

// Removing an inserted key returns its value, and the key is gone afterwards.
void test_remove_existing(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, string_elem("abc"), &result));
  CU_ASSERT_EQUAL(result.i, 123);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, string_elem("abc"), &result));

  ioopm_hash_table_destroy(ht);
}

// Removing a key that was never inserted fails and leaves result untouched.
void test_remove_nonexisting(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, string_elem("abc"), &result));
  CU_ASSERT_EQUAL(result.i, 0);

  ioopm_hash_table_destroy(ht);
}

// Checks for a key in an empty hashtable
void test_has_key_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("abc")));
  
  ioopm_hash_table_destroy(ht);

}

void test_has_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("abc")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("cba")));

  ioopm_hash_table_destroy(ht);
}

void test_has_multiple_keys(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_insert(ht, string_elem("hej"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(2));
  ioopm_hash_table_insert(ht, string_elem("it"),  int_elem(3));
  
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("abc")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("hej")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("it")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht,string_elem("cba")));

 ioopm_hash_table_destroy(ht); 
}

void test_insert_remove_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_remove(ht, string_elem("abc"), &result);

  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, string_elem("abc")));

  ioopm_hash_table_destroy(ht);
}

void test_has_some_keys(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("hej"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(2));
  ioopm_hash_table_insert(ht, string_elem("it"), int_elem(3));

  ioopm_hash_table_remove(ht, string_elem("abc"), &result);

  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("hej")));
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht,string_elem("abc")));
  CU_ASSERT_TRUE(ioopm_hash_table_has_key(ht, string_elem("it")));

  ioopm_hash_table_destroy(ht);
}

void test_is_empty_new(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_is_empty_after_insert(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  CU_ASSERT_FALSE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_is_empty_after_remove(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_remove(ht, string_elem("abc"), &result);
  CU_ASSERT_TRUE(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

// An iterator over an empty hash table is at the end immediately
void test_iterator_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// An iterator over a hash table with one entry visits exactly that entry
void test_iterator_single_entry(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(123));
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);

  CU_ASSERT_FALSE_FATAL(ioopm_hash_table_iterator_at_end(it));

  CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it).s, "abc");
  CU_ASSERT_EQUAL(ioopm_hash_table_iterator_current_value(it).i, 123);

  ioopm_hash_table_iterator_advance(it);
  CU_ASSERT_TRUE(ioopm_hash_table_iterator_at_end(it));

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

/// NOTE: FOLLOWING THREE TEST CASES ARE GENERATED WITH USE OF AI ///

// An iterator visits as many entries as there are in the table
void test_iterator_several_entries(void)
{
  char *keys[3] = {"abc", "qwe", "asd"};
  int values[3] = {0, 1, 2};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  for (int i = 0; i < 3; ++i)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem(values[i]));
  }

  int iteration_count = 0;
  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    iteration_count++;
    ioopm_hash_table_iterator_advance(it);
  }
  CU_ASSERT_EQUAL(iteration_count, 3);

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// Every key-value pair is visited exactly once, with the correct value
void test_iterator_visits_each_entry_once(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(0));
  ioopm_hash_table_insert(ht, string_elem("qwe"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("asd"), int_elem(2));

  int abc_count = 0;
  int qwe_count = 0;
  int asd_count = 0;

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    char *key = ioopm_hash_table_iterator_current_key(it).s;
    int value = ioopm_hash_table_iterator_current_value(it).i;

    if (strcmp(key, "abc") == 0)
    {
      abc_count++;
      CU_ASSERT_EQUAL(value, 0);
    }
    else if (strcmp(key, "qwe") == 0)
    {
      qwe_count++;
      CU_ASSERT_EQUAL(value, 1);
    }
    else if (strcmp(key, "asd") == 0)
    {
      asd_count++;
      CU_ASSERT_EQUAL(value, 2);
    }
    else
    {
      CU_FAIL("iterator returned an unknown key");
    }

    ioopm_hash_table_iterator_advance(it);
  }

  CU_ASSERT_EQUAL(abc_count, 1);
  CU_ASSERT_EQUAL(qwe_count, 1);
  CU_ASSERT_EQUAL(asd_count, 1);

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

// With more keys than buckets, some must share a bucket;
// every key should still be visited exactly once
void test_iterator_same_bucket(void)
{
  char *keys[20] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
                    "k", "l", "m", "n", "o", "p", "q", "r", "s", "t"};
  int n = 20;
  int visited[20] = {0};

  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  for (int i = 0; i < n; ++i)
  {
    ioopm_hash_table_insert(ht, string_elem(keys[i]), int_elem(i));
  }

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    // the value is the key's index in the array
    int value = ioopm_hash_table_iterator_current_value(it).i;
    CU_ASSERT_FATAL(value >= 0 && value < n);
    CU_ASSERT_STRING_EQUAL(ioopm_hash_table_iterator_current_key(it).s, keys[value]);
    visited[value]++;

    ioopm_hash_table_iterator_advance(it);
  }

  for (int i = 0; i < n; ++i)
  {
    CU_ASSERT_EQUAL(visited[i], 1);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

/// NOTE: PREVIOUS THREE TESTCASES ARE GENERATED USING AI ///

void test_size_empty(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);

  ioopm_hash_table_destroy(ht);
}

void test_size_single(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

  ioopm_hash_table_destroy(ht);
}

void test_size_several(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("def"), int_elem(2));
  ioopm_hash_table_insert(ht, string_elem("ghi"), int_elem(3));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(4));   // overwrite, not a new entry
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 3);

  ioopm_hash_table_destroy(ht);
}

void test_size_after_remove(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, string_elem("abc"), int_elem(1));
  ioopm_hash_table_insert(ht, string_elem("def"), int_elem(2));
  ioopm_hash_table_remove(ht, string_elem("abc"), &result);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);

  ioopm_hash_table_destroy(ht);
}

// NOTE: FOLLOWING TEST CASES ARE GENERATED WITH USE OF AI ///

// Keys can be integers when the table is created with int_hash and int_eq
void test_generic_int_keys(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  elem_t result = int_elem(0);

  ioopm_hash_table_insert(ht, int_elem(1), int_elem(100));
  ioopm_hash_table_insert(ht, int_elem(2), int_elem(200));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(1), &result));
  CU_ASSERT_EQUAL(result.i, 100);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(2), &result));
  CU_ASSERT_EQUAL(result.i, 200);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(3)));

  ioopm_hash_table_destroy(ht);
}

// Integer keys that differ by the number of buckets end up in the same
// bucket; all of them must still be found, and removing one leaves the others
void test_generic_int_keys_same_bucket(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  elem_t result = int_elem(0);

  // with 17 buckets, 3, 20 and 37 all hash to bucket 3
  ioopm_hash_table_insert(ht, int_elem(3), int_elem(1));
  ioopm_hash_table_insert(ht, int_elem(20), int_elem(2));
  ioopm_hash_table_insert(ht, int_elem(37), int_elem(3));

  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, int_elem(20), &result));
  CU_ASSERT_EQUAL(result.i, 2);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(3), &result));
  CU_ASSERT_EQUAL(result.i, 1);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(37), &result));
  CU_ASSERT_EQUAL(result.i, 3);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, int_elem(20)));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 2);

  ioopm_hash_table_destroy(ht);
}

// Values can be strings
void test_generic_string_values(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  elem_t result = ptr_elem(NULL);

  ioopm_hash_table_insert(ht, int_elem(1), string_elem("ett"));
  ioopm_hash_table_insert(ht, int_elem(2), string_elem("två"));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, int_elem(2), &result));
  CU_ASSERT_STRING_EQUAL(result.s, "två");

  ioopm_hash_table_destroy(ht);
}

typedef struct
{
  char *name;
  int age;
} person_t;

// Values can be pointers to arbitrary data, stored through void *
void test_generic_struct_values(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  person_t alice = { .name = "Alice", .age = 30 };
  person_t bob = { .name = "Bob", .age = 25 };
  elem_t result = ptr_elem(NULL);

  ioopm_hash_table_insert(ht, string_elem("alice"), ptr_elem(&alice));
  ioopm_hash_table_insert(ht, string_elem("bob"), ptr_elem(&bob));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem("bob"), &result));
  person_t *found = result.p;   // the table doesn't know the type; the caller does
  CU_ASSERT_PTR_EQUAL(found, &bob);
  CU_ASSERT_EQUAL(found->age, 25);
  CU_ASSERT_STRING_EQUAL(found->name, "Bob");

  ioopm_hash_table_destroy(ht);
}

// With string_eq (equivalence), a key is found through a different pointer
// to a string with the same characters
void test_generic_equivalent_keys_are_found(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_hash, string_eq);
  elem_t result = int_elem(0);
  char *stored = strdup("hej");
  char *lookup_key = strdup("hej");   // a different object, same content

  ioopm_hash_table_insert(ht, string_elem(stored), int_elem(42));
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, string_elem(lookup_key), &result));
  CU_ASSERT_EQUAL(result.i, 42);

  ioopm_hash_table_destroy(ht);
  free(stored);
  free(lookup_key);
}

// With ptr_eq (identity), only the very same pointer finds the entry;
// an equivalent but different string does not
void test_generic_identity_keys(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(pointer_hash, pointer_eq);
  elem_t result = int_elem(0);
  char *stored = strdup("hej");
  char *other = strdup("hej");

  ioopm_hash_table_insert(ht, ptr_elem(stored), int_elem(42));

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, ptr_elem(stored), &result));
  CU_ASSERT_EQUAL(result.i, 42);
  CU_ASSERT_FALSE(ioopm_hash_table_has_key(ht, ptr_elem(other)));

  ioopm_hash_table_destroy(ht);
  free(stored);
  free(other);
}

// The iterator returns generic keys and values that can be read back
void test_generic_iterator_int_keys(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(int_hash, int_eq);
  int visited[3] = {0};

  for (int i = 0; i < 3; ++i)
  {
    ioopm_hash_table_insert(ht, int_elem(i), int_elem(i * 10));
  }

  ioopm_hash_table_iterator_t *it = ioopm_hash_table_iterator_create(ht);
  while (!ioopm_hash_table_iterator_at_end(it))
  {
    int key = ioopm_hash_table_iterator_current_key(it).i;
    int value = ioopm_hash_table_iterator_current_value(it).i;
    CU_ASSERT_FATAL(key >= 0 && key < 3);
    CU_ASSERT_EQUAL(value, key * 10);
    visited[key]++;
    ioopm_hash_table_iterator_advance(it);
  }

  for (int i = 0; i < 3; ++i)
  {
    CU_ASSERT_EQUAL(visited[i], 1);
  }

  ioopm_hash_table_iterator_destroy(it);
  ioopm_hash_table_destroy(ht);
}

/// NOTE: PREVIOUS TEST CASES ARE GENERATED USING AI ///

int main(void) {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("Tests related to hash tables", init_suite, clean_suite);
  if (my_test_suite == NULL) {
      // If the test suite could not be added, tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
  }

  // This is where we add the test functions to our test suite.
  // For each call to CU_add_test we specify the test suite, the
  // name or description of the test, and the function that runs
  // the test in question. If you want to add another test, just
  // copy a line below and change the information
  if (
    (CU_add_test(my_test_suite, "Test create destroy", test_create_destroy) == NULL) ||
    (CU_add_test(my_test_suite, "Test insert once", test_insert_once) == NULL) ||
    (CU_add_test(my_test_suite, "Insert existing key overwrites value", test_insert_existing_key) == NULL) ||
    (CU_add_test(my_test_suite, "Insert two different keys", test_insert_new_new) == NULL) ||
    (CU_add_test(my_test_suite, "Insert existing then new key", test_insert_existing_new) == NULL) ||
    (CU_add_test(my_test_suite, "Overwrite same key several times", test_insert_existing_existing) == NULL) ||
    (CU_add_test(my_test_suite, "Remove existing key", test_remove_existing) == NULL) ||
    (CU_add_test(my_test_suite, "Remove nonexisting key", test_remove_nonexisting) == NULL) ||
    (CU_add_test(my_test_suite, "Checks if hashtable is has key while empty", test_has_key_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Checks for two keys in a ht, while only one has been inserted", test_has_key) == NULL) ||
    (CU_add_test(my_test_suite, "Checks for multiple keys in a hashtable", test_has_multiple_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Checks for a key that has been removed", test_insert_remove_key) == NULL) ||
    (CU_add_test(my_test_suite, "Checks for inserted/inserted & removed keys", test_has_some_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Is empty on new table", test_is_empty_new) == NULL) ||
    (CU_add_test(my_test_suite, "Is empty after insert", test_is_empty_after_insert) == NULL) ||
    (CU_add_test(my_test_suite, "Is empty after insert and remove", test_is_empty_after_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator on empty table", test_iterator_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator on single entry", test_iterator_single_entry) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator on several entries", test_iterator_several_entries) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator visits each entry once", test_iterator_visits_each_entry_once) == NULL) ||
    (CU_add_test(my_test_suite, "Iterator on entries in same bucket", test_iterator_same_bucket) == NULL) ||
    (CU_add_test(my_test_suite, "Size of empty table", test_size_empty) == NULL) ||
    (CU_add_test(my_test_suite, "Size of table with one entry", test_size_single) == NULL) ||
    (CU_add_test(my_test_suite, "Size of table with several entries", test_size_several) == NULL) ||
    (CU_add_test(my_test_suite, "Size after remove", test_size_after_remove) == NULL) ||
    (CU_add_test(my_test_suite, "Generic: integer keys", test_generic_int_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Generic: integer keys in same bucket", test_generic_int_keys_same_bucket) == NULL) ||
    (CU_add_test(my_test_suite, "Generic: string values", test_generic_string_values) == NULL) ||
    (CU_add_test(my_test_suite, "Generic: struct values via void pointer", test_generic_struct_values) == NULL) ||
    (CU_add_test(my_test_suite, "Generic: equivalent keys are found", test_generic_equivalent_keys_are_found) == NULL) ||
    (CU_add_test(my_test_suite, "Generic: identity keys", test_generic_identity_keys) == NULL) ||
    (CU_add_test(my_test_suite, "Generic: iterator with integer keys", test_generic_iterator_int_keys) == NULL) ||
    0
  )
    {
      // If adding any of the tests fails, we tear down CUnit and exit
      CU_cleanup_registry();
      return CU_get_error();
    }

  // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
  // Use CU_BRM_NORMAL to only print errors and a summary
  CU_basic_set_mode(CU_BRM_VERBOSE);

  // This is where the tests are actually run!
  CU_basic_run_tests();

  // Tear down CUnit before exiting
  CU_cleanup_registry();
  return CU_get_error();
}