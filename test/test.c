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

void test_ht_custom_hash_function() {
    t_hash_table* ht = ht_new_c_hash(5, &my_hash);
    ht_insert(ht, "HE", "abc", 2, 3);
    ht_destroy(ht);
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

void test_ht_delete() {
    t_hash_table* ht = ht_new(5);
    ht_insert(ht, "HE", "abc", 2, 3);
    int first = ht_delete(ht, "HE", 2);
    int second = ht_delete(ht, "HE", 2);

    if (first != HT_DELETED) {
        fprintf(stderr, "[HashTable::ht_delete] Error\nExpected element %s to be deleted\n", "HE");
    }

    if (second != HT_NOT_CONTAINS) {
        fprintf(stderr, "[HashTable::ht_delete] Error\nExpected element %s to be not present\n", "HE");
    }

    ht_destroy(ht);
}

void test_ht_resizing()  {
    t_hash_table* ht = ht_new(8);

    for (int i = 0; i < 8; i++) {
        char* key = malloc(sizeof(char));
        key[0] = 'A' + i;

        ht_insert(ht, key, "ABCDEF", 1 , 6);

        free(key);
    }

    ht_destroy(ht);
}

void test_ht_foreach() {

    t_hash_table* ht = ht_new(HT_INIT_CAPACITY);

    ht_insert(ht, "abc", "123", 3, 3);
    ht_insert(ht, "def", "456", 3, 3);
    ht_insert(ht, "ghk", "678", 3, 3);
    ht_insert(ht, "lmn", "910", 3, 3);

    FOREACH(k, v, ht, {
          char* key = (char*)k;
          char* val = (char*)v;
          printf("%s=%s\n", key, val);
    })
}

int main() {
    // test_ht_delete();
    test_ht_foreach();
    return 0;
}