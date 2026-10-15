COMPILATOR = gcc
CFLAGS = -Wall -g -Wextra -Wpedantic
COV = --coverage

# Executables

hash_table_tests: hash_table_tests.o hash_table.o hash_table_iterator.o
	$(COMPILATOR) $(CFLAGS) $^ -o $@ -lcunit

linked_list_tests: linked_list_tests.o linked_list.o list_iterator.o
	$(COMPILATOR) $(CFLAGS) $^ -o $@ -lcunit

freq_count: hash_table_freq_count.o hash_table.o hash_table_iterator.o
	$(COMPILATOR) $(CFLAGS) $^ -o $@

# Object files

hash_table.o:						hash_table.c hash_table.h hash_table_internal.h
hash_table_tests.o: 				hash_table_tests.c hash_table.h hash_table_iterator.h
hash_table_iterator.o:				hash_table_iterator.c hash_table_iterator.h hash_table.h hash_table_internal.h
hash_table_freq_count.o:			hash_table_freq_count.c hash_table.h hash_table_iterator.h

linked_list.o:						linked_list.c linked_list.h linked_list_internal.h
linked_list_tests.o:				linked_list_tests.c linked_list.h list_iterator.h
list_iterator.o:					list_iterator.c list_iterator.h linked_list.h linked_list_internal.h

test: hash_table_tests linked_list_tests
	./hash_table_tests
	./linked_list_tests

valgrind_test: hash_table_tests linked_list_tests
	valgrind --leak-check=full ./hash_table_tests
	valgrind --leak-check=full ./linked_list_tests

clean:
	rm -f *.o hash_table_tests linked_list_tests freq_count

.PHONY: 