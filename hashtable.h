#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _HT_PRIME 251
#define _HT_MODULUS 1000000009

// Represents an entry of a hashtable
typedef struct hash_table_entry {
    struct hash_table_entry* next;

    size_t key_size;
    size_t value_size;

    void*   key;
    void*   value;
} t_hash_table_entry;

// Represents a hashtable with the usual operations
typedef struct hash_table {
    size_t capacity;
    t_hash_table_entry** slots;

    // Hash function used to compute the hash for a key
    size_t (*hash_function) (void* key, size_t key_size);

} t_hash_table;

// Initialize a hash table with the specified number of slots.
t_hash_table* ht_new(size_t capacity);
t_hash_table* ht_new_c_hash(size_t capacity, size_t (*f)(void*, size_t));

// Insert element into hash table
void ht_insert(t_hash_table* ht, void* key, void* value, size_t key_size, size_t value_size);

// Looks for a entry that matches this key
// Returns 1 if the corresponding element is found, 0 if not
int ht_contains(t_hash_table* ht, void* key, size_t key_size);

// Look up the value that matches this key.
// If the hashtable does not contain an element with this key, NULL is returned
void ht_lookup(t_hash_table* ht, void* key, size_t key_size, void** value, size_t* value_size);

// Private functions

// Checks if two keys match
int _ht_key_match(void* k1, size_t k1_size, void* k2, size_t k2_size);

size_t _ht_hash(void* mem_location, size_t length);

#endif
