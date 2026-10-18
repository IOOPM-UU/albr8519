#include <CUnit/Basic.h>
#include <string.h>
#include "linked_list.h"
#include "list_iterator.h"
#include <stdbool.h>
#include "common.h"

/// NOTE: gcc -Wall -Wextra -g linked_list.c list_iterator.c linked_list_tests.c -o linked_list_tests -lcunit && ./linked_list_tests

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
  CU_ASSERT_EQUAL(ioopm_list_size(ls), 0);

  ioopm_list_destroy(ls);
}

void test_destroy_one_element(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(18));

  CU_ASSERT_FALSE(ioopm_list_is_empty(ls));
  CU_ASSERT_EQUAL(ioopm_list_size(ls), 1);
  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 18);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 18);

  ioopm_list_destroy(ls);
}

void test_append_one_element(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(5));

  CU_ASSERT_EQUAL(ioopm_list_size(ls), 1);
  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 5);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 5);

  ioopm_list_destroy(ls);
}

void test_append_multiple_elements(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));
  ioopm_list_append(ls, int_elem(5));
  ioopm_list_append(ls, int_elem(8));

  CU_ASSERT_EQUAL(ioopm_list_size(ls), 3);
  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 3);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 8);

  CU_ASSERT_EQUAL(ioopm_list_get(ls, 0).i, 3); 
  CU_ASSERT_EQUAL(ioopm_list_get(ls, 1).i, 5); 
  CU_ASSERT_EQUAL(ioopm_list_get(ls, 2).i, 8);

  ioopm_list_destroy(ls);
}

void test_prepend_one_element(void)
{
  ioopm_list_t *ls = ioopm_list_create();

  ioopm_list_prepend(ls, int_elem(5));
  ioopm_list_prepend(ls, int_elem(10));

  CU_ASSERT_EQUAL(ioopm_list_size(ls), 2);
  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 10);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 5);

  ioopm_list_destroy(ls);
}

void test_prepend_multiple_elements(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_prepend(ls, int_elem(3));
  ioopm_list_prepend(ls, int_elem(5));
  ioopm_list_prepend(ls, int_elem(8));

  CU_ASSERT_EQUAL(ioopm_list_size(ls), 3);
  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 8);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 3);

  CU_ASSERT_EQUAL(ioopm_list_get(ls, 0).i, 8); 
  CU_ASSERT_EQUAL(ioopm_list_get(ls, 1).i, 5); 
  CU_ASSERT_EQUAL(ioopm_list_get(ls, 2).i, 3);

  ioopm_list_destroy(ls);
}

void test_list_get_head(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));
  ioopm_list_append(ls, int_elem(5));

  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 3);
  CU_ASSERT_NOT_EQUAL(ioopm_list_head(ls).i, 5);

  ioopm_list_destroy(ls);
}

void test_list_get_last(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));
  ioopm_list_append(ls, int_elem(5));

  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 5);
  CU_ASSERT_NOT_EQUAL(ioopm_list_last(ls).i, 3);

  ioopm_list_destroy(ls);
}

void test_insert_one_element(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));

  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 3);

  ioopm_list_insert(ls, 0, int_elem(5));
  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 5);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 3);

  ioopm_list_destroy(ls);
}

void test_insert_multiple_elements(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));
  ioopm_list_append(ls, int_elem(5));

  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 3);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 5);

  ioopm_list_insert(ls, 0, int_elem(8));
  ioopm_list_insert(ls, 3, int_elem(11));

  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 8);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 11);

  ioopm_list_destroy(ls);
}

void test_remove_one_element(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));
  ioopm_list_append(ls, int_elem(5));
  ioopm_list_append(ls, int_elem(8));
  ioopm_list_append(ls, int_elem(11));

  CU_ASSERT_EQUAL(ioopm_list_get(ls, 1).i, 5)
  ioopm_list_remove(ls, 1);

  CU_ASSERT_EQUAL(ioopm_list_get(ls, 1).i, 8)
  ioopm_list_remove(ls, 1);

  CU_ASSERT_EQUAL(ioopm_list_get(ls, 1).i, 11)
  CU_ASSERT_EQUAL(ioopm_list_head(ls).i, 3);
  CU_ASSERT_EQUAL(ioopm_list_last(ls).i, 11);

  ioopm_list_destroy(ls);
}

void test_list_get_element(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));
  ioopm_list_append(ls, int_elem(5));

  CU_ASSERT_EQUAL(ioopm_list_get(ls, 0).i, 3)
  CU_ASSERT_EQUAL(ioopm_list_get(ls, 1).i, 5)

  ioopm_list_destroy(ls);
}

void test_list_size(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, int_elem(3));
  ioopm_list_append(ls, int_elem(5));
  ioopm_list_append(ls, int_elem(8));
  ioopm_list_append(ls, int_elem(11));

  CU_ASSERT_EQUAL(ioopm_list_size(ls), 4);

  ioopm_list_destroy(ls);
}

/// NOTE: FOLLOWING TEST CASES ARE GENERATED WITH USE OF AI ///

// The list can store strings
void test_generic_list_strings(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, string_elem("ett"));
  ioopm_list_append(ls, string_elem("två"));
  ioopm_list_prepend(ls, string_elem("noll"));

  CU_ASSERT_EQUAL(ioopm_list_size(ls), 3);
  CU_ASSERT_STRING_EQUAL(ioopm_list_head(ls).s, "noll");
  CU_ASSERT_STRING_EQUAL(ioopm_list_get(ls, 1).s, "ett");
  CU_ASSERT_STRING_EQUAL(ioopm_list_last(ls).s, "två");

  ioopm_list_destroy(ls);
}

// The list can store booleans and floats
void test_generic_list_bool_and_float(void)
{
  ioopm_list_t *bools = ioopm_list_create();
  ioopm_list_append(bools, bool_elem(true));
  ioopm_list_append(bools, bool_elem(false));
  CU_ASSERT_TRUE(ioopm_list_get(bools, 0).b);
  CU_ASSERT_FALSE(ioopm_list_get(bools, 1).b);
  ioopm_list_destroy(bools);

  ioopm_list_t *floats = ioopm_list_create();
  ioopm_list_append(floats, (elem_t) { .f = 1.5f });
  ioopm_list_append(floats, (elem_t) { .f = -2.25f });
  CU_ASSERT_DOUBLE_EQUAL(ioopm_list_head(floats).f, 1.5, 0.0001);
  CU_ASSERT_DOUBLE_EQUAL(ioopm_list_last(floats).f, -2.25, 0.0001);
  ioopm_list_destroy(floats);
}

typedef struct
{
  char *name;
  int age;
} person_t;

// The list can store pointers to arbitrary data through void *
void test_generic_list_struct_pointers(void)
{
  person_t alice = { .name = "Alice", .age = 30 };
  person_t bob = { .name = "Bob", .age = 25 };

  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, ptr_elem(&alice));
  ioopm_list_append(ls, ptr_elem(&bob));

  person_t *first = ioopm_list_head(ls).p;   // the list doesn't know the type; we do
  CU_ASSERT_PTR_EQUAL(first, &alice);
  CU_ASSERT_STRING_EQUAL(first->name, "Alice");

  person_t *second = ioopm_list_get(ls, 1).p;
  CU_ASSERT_EQUAL(second->age, 25);

  ioopm_list_destroy(ls);
}

// The list stores pointers, not copies: a change through the original
// is visible through the list (aliasing)
void test_generic_list_stores_pointer_not_copy(void)
{
  person_t alice = { .name = "Alice", .age = 30 };

  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, ptr_elem(&alice));

  alice.age = 31;   // modify the original after inserting
  person_t *stored = ioopm_list_head(ls).p;
  CU_ASSERT_EQUAL(stored->age, 31);

  ioopm_list_destroy(ls);
}

// Insert and remove work with generic elements, and remove returns the element
void test_generic_list_insert_remove_strings(void)
{
  ioopm_list_t *ls = ioopm_list_create();
  ioopm_list_append(ls, string_elem("a"));
  ioopm_list_append(ls, string_elem("c"));
  ioopm_list_insert(ls, 1, string_elem("b"));   // a, b, c

  CU_ASSERT_STRING_EQUAL(ioopm_list_get(ls, 1).s, "b");

  elem_t removed = ioopm_list_remove(ls, 0);     // b, c
  CU_ASSERT_STRING_EQUAL(removed.s, "a");
  CU_ASSERT_STRING_EQUAL(ioopm_list_head(ls).s, "b");
  CU_ASSERT_EQUAL(ioopm_list_size(ls), 2);

  ioopm_list_destroy(ls);
}

// The iterator returns generic elements in order
void test_generic_list_iterator_strings(void)
{
  char *expected[3] = {"x", "y", "z"};

  ioopm_list_t *ls = ioopm_list_create();
  for (int i = 0; i < 3; ++i)
  {
    ioopm_list_append(ls, string_elem(expected[i]));
  }

  int i = 0;
  ioopm_list_iterator_t *it = ioopm_list_iterator_create(ls);
  while (!ioopm_list_iterator_at_end(it))
  {
    CU_ASSERT_FATAL(i < 3);
    CU_ASSERT_STRING_EQUAL(ioopm_list_iterator_current(it).s, expected[i]);
    ++i;
    ioopm_list_iterator_advance(it);
  }
  CU_ASSERT_EQUAL(i, 3);

  ioopm_list_iterator_destroy(it);
  ioopm_list_destroy(ls);
}

/// NOTE: PREVIOUS TEST CASES ARE GENERATED USING AI ///


int main(void) {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("Test suite for linked lists ", init_suite, clean_suite);
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
    (CU_add_test(my_test_suite, "Adds one element at the end of a list", test_append_one_element) == NULL) ||
    (CU_add_test(my_test_suite, "Adds three elements at the end, tests get function aswell", test_append_multiple_elements) == NULL) ||
    (CU_add_test(my_test_suite, "Adds one element at the beginning of a list", test_prepend_one_element) == NULL) ||
    (CU_add_test(my_test_suite, "Adds three elements at the beginning of a list", test_prepend_multiple_elements) == NULL) ||
    (CU_add_test(my_test_suite, "Fetches the first element of a list", test_list_get_head) == NULL) ||
    (CU_add_test(my_test_suite, "Fetches the last element of a list", test_list_get_last) == NULL) ||
    (CU_add_test(my_test_suite, "Inserts one element into a list", test_insert_one_element) == NULL) ||
    (CU_add_test(my_test_suite, "Inserts multiple element into a list", test_insert_multiple_elements) == NULL) ||
    (CU_add_test(my_test_suite, "Removes one element from a list", test_remove_one_element) == NULL) ||
    (CU_add_test(my_test_suite, "Fetches a element from a list", test_list_get_element) == NULL) ||
    (CU_add_test(my_test_suite, "Checks the size of a list is equal to amount of elememts", test_list_size) == NULL) ||
    (CU_add_test(my_test_suite, "Generic list: strings", test_generic_list_strings) == NULL) ||
    (CU_add_test(my_test_suite, "Generic list: bool and float", test_generic_list_bool_and_float) == NULL) ||
    (CU_add_test(my_test_suite, "Generic list: struct pointers", test_generic_list_struct_pointers) == NULL) ||
    (CU_add_test(my_test_suite, "Generic list: stores pointer, not copy", test_generic_list_stores_pointer_not_copy) == NULL) ||
    (CU_add_test(my_test_suite, "Generic list: insert and remove strings", test_generic_list_insert_remove_strings) == NULL) ||
    (CU_add_test(my_test_suite, "Generic list: iterator over strings", test_generic_list_iterator_strings) == NULL) ||
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