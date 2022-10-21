#include "hashtable.h"

t_hash_table* ht_new(size_t capacity) {
    assert(capacity > 0);

    t_hash_table* table = (t_hash_table*)malloc(sizeof(t_hash_table));
    table->capacity = capacity;
    table->hash_function = &_ht_hash;

    table->slots = (t_hash_table_entry**)calloc(capacity, sizeof(t_hash_table_entry*));

    return table;
}

t_hash_table* ht_new_c_hash(size_t capacity, size_t (*f)(void*, size_t)) {
    t_hash_table* ht = ht_new(capacity);
    ht->hash_function = f;
    return ht;
}

void ht_insert(t_hash_table* ht, void* key, void* value, size_t key_size, size_t value_size) {
    size_t hash = ht->hash_function(key, key_size);
    size_t position = (hash) % ht->capacity;

    t_hash_table_entry* entry = malloc(sizeof(t_hash_table_entry));
    entry->key = key;
    entry->key_size = key_size;
    entry->value = value;
    entry->value_size = value_size;

    // If there exists no entry, then set new entry
    if (ht->slots[position] != NULL) {
        entry->next = ht->slots[position];
    } else {
        entry->next = NULL;
    }

    ht->slots[position] = entry;
}

int ht_contains(t_hash_table* ht, void* key, size_t key_size) {
    size_t position = (ht->hash_function(key, key_size)) % ht->capacity;

    t_hash_table_entry* entry = ht->slots[position];

    while (entry != NULL) {
        if (_ht_key_match(key, key_size, entry->key, entry->key_size)) {
            return 1;
        }

        entry = entry->next;
    }

    return 0;
}


void ht_lookup(t_hash_table* ht, void* key, size_t key_size, void** value, size_t* value_size) {
    size_t position = (ht->hash_function(key, key_size)) % ht->capacity;

    t_hash_table_entry* entry = ht->slots[position];

    while (entry != NULL) {

        entry = entry->next;
    }
}

// Private functions
int _ht_key_match(void* k1, size_t k1_size, void* k2, size_t k2_size) {
    if (k1_size != k2_size) {
        return 0;
    } else {
        return memcmp(k1, k2, k1_size) == 0;
    }
}

size_t _ht_hash(void* mem_location, size_t length) {

    size_t hash = 0;
    size_t p_power = 1;
    for (int i = 0; i < length; i++) {
        char c = ((char*)mem_location)[i];
        hash += (c * p_power) % _HT_MODULUS;
        p_power = (p_power * _HT_PRIME) % _HT_MODULUS;
    }

    return hash;
}