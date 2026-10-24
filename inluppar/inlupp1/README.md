# Assignment 1: Generic hash table and linked list
 
Alexander Brundin (albr8519)


### Running the frequency count program
The frequency count program is supposed to be runned together with the textfiles were we want the program to operate. Start by navigating to the directory where the program is located and preferably make sure that the textfiles is in the same directory. Run the program like the following: 

```
./hash_table_freq_count textfile1.txt textfile2.txt ...
```


### Files and description 
| File | Description | 
|---|---|
| `hash_table.c/.h` | Generic hash table |
| `hash_table_iterator.c/.h` | Iterator for the generic hash table |
| `hash_table_internal.h` | Private file with struct definitions, shared by the hash table and its iterator |
| `linked_list.c/.h` | Generic linked list | 
| `list_iterator.c/.h` | Iterator for linked lists |
| `common.c/.h` | Includes hash- & equality functions aswell as the generic element type elem_t |
| `hash_table_tests.c` | Hash table unit tests (CUnit) |
| `linked_lists_tests.c ` | Linked list unit tests (CUnit) |
| `hash_table_freq_count.c` | Word frequency count program |


### Design & Decisions
- Errors are handled with `assert`. If a precondition documented with `@pre` is broken the program aborts with an error message.
A missing key is not an error, `ioopm_hash_table_lookup` and `ioopm_hash_table_remove` return `false` instead.
- All keys, values and elements have type `elem_t`. Which is a union defined in `common.h`
- The hash table is created with a hash function and an equality function for its keys. The two functions must agree, equal keys must get the same hash
- The linked list keeps pointers to both the first and last link and stores its size, which makes `append`, `prepend`, `head`, `last` & `size` run in O(1). 
- The linked list and hash table own their internal nodes and free them in `remove` and `destroy`. The user is responsible for freeing keys, values and elements that point to allocated memory
- The hash table has a fixed number of buckets (17). This can be changed in `hash_table_internal.h`.
- Iterators become invalid if the underlying data structure is modified.