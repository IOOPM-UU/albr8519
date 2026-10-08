#include <stdlib.h>
#include "list_iterator.h"

struct list_iterator{
    struct link *current;
    ioopm_list_t *list;    
};

/// @brief Create a new iterator
/// @param l the list to iterate over
ioopm_list_iterator_t *ioopm_list_iterator_create(ioopm_list_t *list)
{
    ioopm_list_iterator_t *ls_iterator = calloc(1, sizeof(ioopm_list_iterator_t));

    ls_iterator->list = list;
    ls_iterator->current = list->first;
    return ls_iterator;
}

/// @brief Destroy the iterator and return its resources
/// @param iter the iterator
void ioopm_list_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}

/// @brief Checks if the iterator is positioned at-the-end
/// @param iter the iterator
/// @return true if iter is positioned at-the-end
bool ioopm_list_iterator_at_end(ioopm_list_iterator_t *iter)
{
    return iter->current == NULL;
}

/// @brief Step the iterator forward one step
/// @param iter the iterator
void ioopm_list_iterator_advance(ioopm_list_iterator_t *iter)
{
    assert(!ioopm_list_iterator_at_end(iter) && "Iterator on end");
    iter->current = iter->current->next;
}

/// @brief Return the current element from the underlying list
/// @param iter the iterator
/// @return the current element
int ioopm_list_iterator_current(ioopm_list_iterator_t *iter)
{
    assert(!ioopm_list_iterator_at_end(iter) && "Current is at end");
    return iter->current->value;
}
