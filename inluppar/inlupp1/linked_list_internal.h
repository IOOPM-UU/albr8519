#pragma once
#include "linked_list.h"

typedef struct link link_t;

struct link
{
  elem_t value;
  struct link *next;
};

struct list
{
  struct link *first;
  struct link *last;
  size_t size;
};