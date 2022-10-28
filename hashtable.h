#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _HT_PRIME 251
#define _HT_MODULUS 1000000009

// Return value if element in hash table is updated
#define HT_UPDATED 1

// Return value if element in hash table is inserted
#define HT_INSERTED 2

// Return value when hashtable contains an element
#define HT_CONTAINS 1

// Return value when hashtable not contains an element
#define HT_NOT_CONTAINS 0

typedef size_t (*t_hash_function)(void* key, size_t key_size);


// Represents an entry of a hashtable
typedef struct hash_table_entry {
    struct hash_table_entry* next;
    struct hash_table_entry* previous;

    size_t key_size;
    size_t value_size;

    void*   key;
    void*   value;
} t_hash_table_entry;

// Represents a hashtable with the usual operations
typedef struct hash_table {
    size_t capacity;                // Number of slots in the table
    size_t size;                    // Number of elements stored in the table
    t_hash_table_entry** slots;
    t_hash_function hash_function;      // Hash function used

} t_hash_table;

// Constructors

// Initialize a hashtable with the specified capacity.
t_hash_table* ht_new(size_t capacity);

// Initialize a hashtable with the specified capacity and a custom hash function.
// For an uniform distribution among the slots, a good hash function needs to be chosen.
t_hash_table* ht_new_c_hash(size_t capacity, t_hash_function hash);


// Destructors

// Free all memory occupied by the hashtable
void ht_destroy(t_hash_table* ht);


// Methods

// Insert element into hash table. The memory occupied by the key and value is copied.
// If a key is already present, it updates the corresponding value.
// Returns:
//   HT_INSERTED - if a new element is inserted
//   HT_UPDATED - if a already present element is updated
// Time complexity: O(1)
int ht_insert(t_hash_table* ht, void* key, void* value, size_t key_size, size_t value_size);

// Looks for an entry that matches the provided key.
// Returns:
//   HT_CONTAINS - if element with key is present
//   HT_NOT_CONTAINS - else
// Time complexity: O(1) [Amortized analysis]
int ht_contains(t_hash_table* ht, void* key, size_t key_size);

// Look up the value that matches this key.
// If the hashtable does not contain an element with this key, NULL is returned
// and the value_size parameter is set to 0.
void* ht_lookup(t_hash_table* ht, void* key, size_t key_size, size_t* value_size);

// Private functions

// Checks if two keys match
int _ht_key_match(void* k1, size_t k1_size, void* k2, size_t k2_size);

size_t _ht_hash(void* mem_location, size_t length);

// Destroy a single slot (including the linked-list that is formed by the slot element).
void _ht_destroy_slot(t_hash_table_entry* entry);

#endif
