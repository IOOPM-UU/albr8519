#pragma once
#include <stddef.h>
#include <stdbool.h>

#define int_elem(x)   ((elem_t) { .i = (x) })
#define bool_elem(x)  ((elem_t) { .b = (x) })
#define ptr_elem(x)   ((elem_t) { .p = (x) })
#define string_elem(x) ((elem_t) { .s = (x) })

typedef union elem elem_t;
typedef bool ioopm_eq_function(elem_t a, elem_t b);
typedef size_t ioopm_hash_function(elem_t key);

size_t string_hash(elem_t key);
bool string_eq(elem_t a, elem_t b);
size_t int_hash(elem_t key);
bool int_eq(elem_t a, elem_t b);
size_t pointer_hash(elem_t key);
bool pointer_eq(elem_t a, elem_t b);

union elem
{
    int i;
    unsigned int u;
    bool b;
    float f;
    void *p;
    char *s;
};

