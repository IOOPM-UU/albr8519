#include "linked_list.h"
#include <stdlib.h>
#include <assert.h>

/// @brief Creates a new empty list
/// @return an empty linked list
ioopm_list_t *ioopm_list_create(void)
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    link_t *dummy_node = calloc(1, sizeof(link_t));

    list->first = dummy_node;
    list->last = dummy_node;

    return list;
}

/// @brief Tear down the linked list and return all its memory (but not the memory of the elements)
/// @param list the list to be destroyed
void ioopm_list_destroy(ioopm_list_t* ls)
{
    struct link* current = ls->first;
    while(current != NULL)
    {
        struct link*next = current->next; // Spara undan innan 
        free(current);
        current = next;
    }
    free(ls);
}

static struct link *link_create(int value, struct link *next)
{
    struct link *link = calloc(1, sizeof(link)); 
    link->value = value;
    link->next = next;
    return link;
}

/// @brief Insert at the end of a linked list in O(1) time
/// @param list the linked list that will be appended
/// @param value the value to be appended
void ioopm_list_append(ioopm_list_t *list, int value)
{
    struct link *new_link = link_create(value, NULL);

    if(list->last == NULL)                      // Om listan är tom ändra så både first & last pekar på nya länken
    {
        list->first = new_link;                 
        list->last = new_link;

    } else
    {
        list->last->next = new_link;            // Ändrar nuvarande sista nodens nästa värde
        list->last = new_link;                  // Ändrar så att sista noden pekar på den nya länken
    }
    list->size++;                               // Ökar listans size
}

/// @brief Insert at the front of a linked list in O(1) time
/// @param list the linked list that will be prepended to
/// @param value the value to be prepended
void ioopm_list_prepend(ioopm_list_t *list, int value)
{
    struct link *new_link = link_create(value, list->first);

    if(list->last == NULL)
    {
        list->last = new_link;
    }
    list->first = new_link;
    list->size++;
}

/// @brief Return the first element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the head of
int ioopm_list_head(ioopm_list_t *list)
{
    assert(!ioopm_list_is_empty(list) && "List is empty");
    return list->first->value;
}

/// @brief Return the last element of a linked list in O(1) time
/// @pre the list is non-empty
/// @param list the linked list to take the last element of
int ioopm_list_last(ioopm_list_t *list)
{
    assert(!ioopm_list_is_empty(list) && "List is empty");
    return list->last->value;
}

static struct link *find_previous_link(ioopm_list_t *list, int index)
{
    struct link *current = list->first;
    for(int i = 0; i < index - 1; i++)
    {
        current = current->next;
    }
    return current;
}

/// @brief Insert an element into a linked list in O(n) time.
/// The valid values of index are [0,n] for a list of n elements,
/// where 0 means before the first element and n means after
/// the last element.
/// @pre 0 <= index <= length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @param value the value to be inserted
void ioopm_list_insert(ioopm_list_t *list, int index, int value)
{
    assert(0 <= index && index <= list->size - 1 && "Index to get is out of bounds" );

    if (index == 0)
    {
        ioopm_list_prepend(list, value);
    }
    
    else if (index == ioopm_list_size(list))
    {
        ioopm_list_append(list, value);

    } else
    {
        struct link *previous = find_previous_link(list, index);
        previous->next = link_create(list, previous->next);
        list->size++;
    }
}



/// @brief Remove an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list
/// @param index the position in the list
/// @return the value removed
int ioopm_list_remove(ioopm_list_t *list, int index)
{
    assert(0 <= index && index < list->size && "Index to be removed is out of bounds");

    struct link *previous = NULL;
    struct link *to_remove;

    if(index == 0)
    {
        to_remove = list->first;
        list->first = to_remove->next;
    }
    else
    {
        previous = find_previous_link(list, index);
        to_remove = previous->next;
        previous->next = to_remove->next;
    }

    if (to_remove = list->last)
    {
        list->last = previous;
    }

    int value = to_remove->value;
    free(to_remove);
    list->size--;
    return value;

}

/// @brief Retrieve an element from a linked list in O(n) time.
/// The valid values of index are [0,n-1] for a list of n elements,
/// where 0 means the first element and n-1 means the last element.
/// @pre 0 <= index < length(list)
/// @param list the linked list that will be extended
/// @param index the position in the list
/// @return the value at the given position
int ioopm_list_get(ioopm_list_t *list, int index)
{
    assert(0 <= index && index <= list->size - 1 && "Index to get is out of bounds" );

    struct link *current = list->first;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current->value;
}

/// @brief Lookup the number of elements in the linked list in O(1) time
/// @param list the linked list
/// @return the number of elements in the list
int ioopm_list_size(ioopm_list_t *list)
{
    return list->size;
}

/// @brief Test whether a list is empty or not
/// @param list the linked list
/// @return true if the number of elements int the list is 0, else false
bool ioopm_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}