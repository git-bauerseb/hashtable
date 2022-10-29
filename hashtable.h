#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define _HT_PRIME 251
#define _HT_MODULUS 1000000009

#define HT_UPDATED 1        // element in hash table is updated
#define HT_INSERTED 2       // element in hash table is inserted

#define HT_CONTAINS 1       // hashtable contains an element
#define HT_NOT_CONTAINS 0   // hashtable not contains an element

#define HT_DELETED 1        // element was deleted

#define HT_SUCCESS 1        // Successful operation on hashtable
#define HT_ERROR 0          // Unsuccessful operation on hashtable

#define HT_INIT_CAPACITY 16  // Initial capacity of a hashtable if no
                             // specific value is provided

#define HT_GROWTH_CONST 2    // Multiplier by which the hashtable is enlarged
#define HT_SHRINK_CONST 2    // Multiplier by which the hashtable is reduced

#define HT_MAX_LOAD_FACTOR .75f     // Maximum load factor that is acceptable
                                    // in the hashtable

// Macros for easier use of hashtable
#define FOREACH(key_, value_, table, code) \
    for(int i = 0; i < table->capacity; i++) { \
        for (t_hash_table_entry* entry = table->slots[i]; entry; entry = entry->next) { \
            void* key_ = entry->key;      \
            void* value_ = entry->value;  \
            code                          \
        }                                 \
    }

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

int ht_delete(t_hash_table* ht, void* key, size_t key_size);

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

// Resize the provided hashtable such that it has n_capacity slots afterwards.
// The elements are rehashed accordingly.
int _ht_rehash(t_hash_table* ht, size_t n_capacity);


// Determines whether the hashtable with the given size should be shrinked
int _ht_reduce(t_hash_table* ht);

// Determines whether the hashtable with the current size should be enlarged
int _ht_enlarge(t_hash_table* ht);

#endif
