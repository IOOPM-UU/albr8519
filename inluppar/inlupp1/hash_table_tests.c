#include "hash_table.h"
#include <CUnit/Basic.h>

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
   ioopm_hash_table_t *ht = ioopm_hash_table_create();
   CU_ASSERT_PTR_NOT_NULL(ht);
   ioopm_hash_table_destroy(ht);
}

void test_insert_once(void)
{
  // create new hash table
  ioopm_hash_table_t *ht = ioopm_hash_table_create();

  char *key = "abc";
  int value = 123;

  // check that key is not in ht
  int result = 0;
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, 0);

  // insert key-value pair and check that the mapping exists
  ioopm_hash_table_insert(ht, key, value);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, key, &result));
  CU_ASSERT_EQUAL(result, value);

  // destroy hash table
  ioopm_hash_table_destroy(ht);
}

// Inserting a key that is already in the table overwrites its value instead of adding a duplicate entry
void test_insert_existing_key(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int result = 0;

  ioopm_hash_table_insert(ht, "abc", 1);
  ioopm_hash_table_insert(ht, "abc", 2);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "abc", &result));
  CU_ASSERT_EQUAL(result, 2);

  ioopm_hash_table_destroy(ht);
}

// Two different keys can be stored at the same time, and lookup returns the correct value for each.
void test_insert_new_new(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int result = 0;

  ioopm_hash_table_insert(ht, "abc", 1);
  ioopm_hash_table_insert(ht, "def", 2);

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "abc", &result));
  CU_ASSERT_EQUAL(result, 1);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "def", &result));
  CU_ASSERT_EQUAL(result, 2);

  ioopm_hash_table_destroy(ht);
}

// Inserting a new key after an overwrite does not affect the overwritten entry.
void test_insert_existing_new(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int result = 0;

  ioopm_hash_table_insert(ht, "abc", 1);
  ioopm_hash_table_insert(ht, "abc", 2);   // 2: existing
  ioopm_hash_table_insert(ht, "def", 3);   // 1: new

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "abc", &result));
  CU_ASSERT_EQUAL(result, 2);
  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "def", &result));
  CU_ASSERT_EQUAL(result, 3);

  ioopm_hash_table_destroy(ht);
}

// When the same key is overwritten several times, lookup always returns the most recent value.
void test_insert_existing_existing(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int result = 0;

  ioopm_hash_table_insert(ht, "abc", 1);
  ioopm_hash_table_insert(ht, "abc", 2);   // 2: befintlig
  ioopm_hash_table_insert(ht, "abc", 3);   // 2: befintlig

  CU_ASSERT_TRUE(ioopm_hash_table_lookup(ht, "abc", &result));
  CU_ASSERT_EQUAL(result, 3);

  ioopm_hash_table_destroy(ht);
}

// Removing an inserted key returns its value, and the key is gone afterwards.
void test_remove_existing(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int result = 0;

  ioopm_hash_table_insert(ht, "abc", 123);
  CU_ASSERT_TRUE(ioopm_hash_table_remove(ht, "abc", &result));
  CU_ASSERT_EQUAL(result, 123);
  CU_ASSERT_FALSE(ioopm_hash_table_lookup(ht, "abc", &result));

  ioopm_hash_table_destroy(ht);
}

// Removing a key that was never inserted fails and leaves result untouched.
void test_remove_nonexisting(void)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create();
  int result = 0;

  CU_ASSERT_FALSE(ioopm_hash_table_remove(ht, "abc", &result));
  CU_ASSERT_EQUAL(result, 0);

  ioopm_hash_table_destroy(ht);
}

int main(void) {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
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