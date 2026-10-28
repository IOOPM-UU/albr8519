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

## Code coverage

Generated with `make coverage`, using gcov.

| File | Line coverage | Branch coverage |
|---|---|---|
| hash_table.c | 100.00% | 100.00% |
| hash_table_iterator.c | 100.00% | 78.57% |
| linked_list.c | 97.40% | 75.00% |
| list_iterator.c | 100.00% | 50.00% |
| common.c | 100.00% | 100.00% |
| **Total** | **99.04%** | |

The uncovered branches are mainly the failure cases of `assert`, which the
tests do not trigger since that would abort the test program.

# Initial Profiling Results

## How the numbers were obtained

- Tool: gprof. The program was built with `make profile`, which adds `-pg`
  to the compiler flags.
- Machine: polhem.it.uu.se.
- Inputs: three files with only unique words, created with
  `seq 1 N | sed 's/^/word/' > freq-count-files/Nk.txt` for N = 1 000,
  10 000 and 50 000.
- Commands, for each file:
  ```
  ./freq_count freq-count-files/<file>.txt > /dev/null
  gprof -b ./freq_count gmon.out | head -12
  ```

## Results

| Input | Total time | Top 3 functions (% of time) | Calls to `string_eq` |
|---|---|---|---|
| 1k words | too short to measure | no samples | 57 844 |
| 10k words | 0.03 s | `find_previous_entry` (67 %), `string_eq` (33 %), - | 5 872 386 |
| 50k words | 1.08 s | `find_previous_entry` (65 %), `string_eq` (25 %), `string_hash` (4 %) | 147 008 858 |

For the 1k input gprof collected no samples (it samples every 0.01 s), so
only the call counts are meaningful.

## Answers

**Top 3 functions.** For the inputs large enough to measure,
`find_previous_entry` and `string_eq` dominate, together taking about 90-100 %
of the time. `string_hash` is a distant third for the largest input.

**Our code or library code?** All top functions are our own code. Library
functions such as `strcmp`, `getline` and `strtok` do not appear, partly
because they are not compiled with `-pg`, so gprof cannot attribute time to
them separately. The 4 % reported for `int_hash` and `_init` is most likely
misattributed samples, since `int_hash` is never called by the program.

**Consistent across inputs?** Yes, the same two functions dominate for 10k
and 50k words. They are the functions that walk a bucket's chain and compare
keys, which is the work that grows with the size of the table.

**Trends.** The work grows much faster than the input:

- 10 times more words (1k to 10k) gives about 100 times more calls to
  `string_eq`.
- 5 times more words (10k to 50k) gives about 25 times more calls, and about
  36 times more time.

That is quadratic growth. With a fixed number of buckets (17), the average
chain length grows linearly with the number of unique words, so every lookup
and insert has to compare against more keys.

**As expected?** Yes. We expected the fixed number of buckets to make long
chains the bottleneck, and the call counts confirm it: with 1 000 words, the
chains are on average about 1 000 / 17 ≈ 59 entries long when full, so each
operation compares against roughly 30 keys on average, and 2 000 operations
give roughly 58 000 comparisons, which matches the 57 844 calls.

**How to make it faster.**

1. Let the number of buckets grow with the number of entries (the optional
   part of Step 14), so that chains stay short and lookups take roughly
   constant time. This addresses the quadratic growth itself.
2. `process_word` first calls `lookup` and then `insert`, so each word walks
   its bucket's chain twice (2 000 calls to `find_previous_entry` for 1 000
   words). An operation that finds the entry once and updates or inserts it
   would halve the number of chain walks.