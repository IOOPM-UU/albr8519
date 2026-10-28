COMPILATOR = gcc
CFLAGS = -Wall -g -Wextra -Wpedantic
COV = --coverage

all: hash_table_tests linked_list_tests freq_count

# Executables
hash_table_tests: hash_table_tests.o hash_table.o hash_table_iterator.o common.o
	$(COMPILATOR) $(CFLAGS) $^ -o $@ -lcunit

linked_list_tests: linked_list_tests.o linked_list.o list_iterator.o common.o
	$(COMPILATOR) $(CFLAGS) $^ -o $@ -lcunit

freq_count: hash_table_freq_count.o hash_table.o hash_table_iterator.o common.o
	$(COMPILATOR) $(CFLAGS) $^ -o $@

%.o: %.c
	$(COMPILATOR) $(CFLAGS) -c $< -o $@

# Object files

hash_table.o:            hash_table.c hash_table.h hash_table_internal.h common.h
hash_table_tests.o:      hash_table_tests.c hash_table.h hash_table_iterator.h common.h
hash_table_iterator.o:   hash_table_iterator.c hash_table_iterator.h hash_table.h hash_table_internal.h common.h
hash_table_freq_count.o: hash_table_freq_count.c hash_table.h hash_table_iterator.h common.h

linked_list.o:           linked_list.c linked_list.h linked_list_internal.h common.h
linked_list_tests.o:     linked_list_tests.c linked_list.h list_iterator.h common.h
list_iterator.o:         list_iterator.c list_iterator.h linked_list.h linked_list_internal.h common.h

common.o:				 common.c common.h

test: hash_table_tests linked_list_tests
	./hash_table_tests
	./linked_list_tests

valgrind_test: hash_table_tests linked_list_tests hash_table_freq_count
	valgrind --leak-check=full ./hash_table_tests
	valgrind --leak-check=full ./linked_list_tests
	valgrind --leak-check=full ./hash_table_freq_count textfile_test.txt

coverage: clean
	$(MAKE) CFLAGS="$(CFLAGS) $(COV)" hash_table_tests linked_list_tests
	./hash_table_tests
	./linked_list_tests
	gcov -b hash_table.c hash_table_iterator.c linked_list.c list_iterator.c common.c

profile: clean
	$(MAKE) CFLAGS="$(CFLAGS) -pg" freq_count

clean:
	rm -f *.o *.gcda *.gcno *.gcov gmon.out hash_table_tests linked_list_tests freq_count

.PHONY: all test valgrind_test clean