#include <stdio.h>
#include "hashtable.h"

size_t my_hash(void* key, size_t key_size) {return 1;}

void test_ht_hash() {
    char* key = "HE";
    size_t hash = _ht_hash(key, 2);

    size_t expected = 'H' * 1 + 'E' * _HT_PRIME;

    if (hash != expected) {
        fprintf(stderr, "[HashTable::_ht_hash] Error\nExpected %ld, Got %ld\n", expected, hash);
    }
}

void test_ht_insertion() {
    t_hash_table* ht = ht_new(5);

    ht_insert(ht, "HE", "abc", 2, 3);

    size_t size;
    const void* value;


    if (!(value = ht_lookup(ht, "HE", 2, &size))) {
        fprintf(stderr, "[HashTable::ht_lookup/ht_insert] Error\nExpected value to be present\n");
    }

    if (size != 3
        || strncmp(value, "abc", size) != 0) {
        fprintf(stderr, "[HashTable::ht_lookup/ht_insert] Error\nInvalid key\n");
    }

    // Update value
    ht_insert(ht, "HE", "abcd", 2, 4);


    if (!(value = ht_lookup(ht, "HE", 2, &size))
        || size != 4
        || strncmp(value, "abcd", size) != 0) {
        fprintf(stderr, "[HashTable::ht_lookup/ht_insert] Error\nOld value not updated\n");
    }

    ht_destroy(ht);
}

void test_ht_contains() {
    t_hash_table* ht = ht_new(5);
    ht_insert(ht, "HE", "abc", 2, 3);

    if (!ht_contains(ht, "HE", 2)) {
        fprintf(stderr, "[HashTable::ht_contains] Error\nExpected element %s to be found\n", "HE");
    }

    if (ht_contains(ht, "HA", 2)) {
        fprintf(stderr, "[HashTable::ht_contains] Error\nExpected element %s not to be found\n", "HA");
    }

    ht_destroy(ht);
}

void test_ht_custom_hash_function() {
    t_hash_table* ht = ht_new_c_hash(5, &my_hash);
    ht_insert(ht, "HE", "abc", 2, 3);
    ht_destroy(ht);
}

int main() {
    test_ht_insertion();
    return 0;
}