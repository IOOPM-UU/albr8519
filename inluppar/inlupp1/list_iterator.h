#pragma once
#include <stdbool.h>
#include "linked_list.h"

/**
 * @file list_iterator.h
 * @author Alexander Brundin
 * @date 8 October 2026
 * @brief An iterator for visiting the elements of a linked list in order.
 * 
 * An iterator is either positioned at an element, called the current
 * element, or positioned at-the-end, once all elements have been visited.
 * Elements are visited in list order, from the first to the last.
 * 
 * Preconditions marked @pre are checked with assert: breaking one aborts
 * the program with an error message.
 */

typedef struct list_iterator ioopm_list_iterator_t;

/// @brief Create an iterator for a list
/// @param list the list to iterate over
/// @return a new iterator positioned at the first element, or at-the-end if
/// the list is empty; to be freed with ioopm_list_iterator_destroy
ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *list);

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter);

/// @brief Checks if the iterator is positioned at-the-end
/// @param iter the iterator
/// @return true if iter is positioned at-the-end
bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter);

/// @brief Step the iterator forward one step
/// @param iter the iterator
void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter);

/// @brief Check whether the iterator is positioned at-the-end
/// @param iter the iterator
/// @return true if all elements have been visited (there is no current
/// element), false otherwise
elem_t ioopm_list_iterator_current(ioopm_list_iterator_t *iter);

/// @brief Move the iterator to the next element, or to at-the-end if there
/// are no more elements
/// @pre iter is not at-the-end
/// @param iter the iterator
elem_t ioopm_list_iterator_remove(ioopm_list_iterator_t *iter);

/// @brief Get the current element
/// @pre iter is not at-the-end
/// @param iter the iterator
/// @return the current element
void ioopm_list_iterator_insert(ioopm_list_iterator_t *iter, elem_t element);