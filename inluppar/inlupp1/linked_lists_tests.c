#include <CUnit/Basic.h>
#include <string.h>
#include "linked_list.h"

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

void test_create_derstroy_list(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  CU_ASSERT_TRUE(ioopm_list_is_empty(ls));

  ioopm_list_destroy(ls);
}

void test_validate_empty_list_size(void)
{
  ioopm_list_t * ls = ioopm_list_create();

  CU_ASSERT_EQUAL(ioopm_list_size(ls), 0);

  ioopm_list_destroy(ls);
}

void test_destroy_empty_list(void)
{
  ioopm_list_t * ls = ioopm_list_create();

  CU_ASSERT_PTR_NOT_NULL(ls);
  CU_ASSERT_TRUE(ioopm_list_is_empty(ls));
  CU_ASSERT_TRUE(ioopm_list_size(ls), 0);

  ioopm_list_destroy(ls);
}

void test_destroy_one_element(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, 18);

  CU_ASSERT_FALSE(ioopm_list_is_empty(ls));
  CU_ASSERT_EQUAL(ioopm_list_size(ls), 1);
  CU_ASSERT_EQUAL(ioopm_list_head(ls), 42);
  CU_ASSERT_EQUAL(ioopm_list_last(ls), 42);

  ioopm_list_destroy(ls);
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
    (CU_add_test(my_test_suite, "Test create empty list", test_create_derstroy_list) == NULL) ||
    (CU_add_test(my_test_suite, "Ensure that an empty list has size 0", test_validate_empty_list_size) == NULL) ||
    (CU_add_test(my_test_suite, "Destroys an empty list", test_destroy_empty_list) == NULL) ||
    (CU_add_test(my_test_suite, "Destroys a list with one element", test_destroy_one_element) == NULL) ||
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