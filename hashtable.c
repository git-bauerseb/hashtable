#include "hashtable.h"

t_hash_table* ht_new(size_t capacity) {
    assert(capacity > 0);

    t_hash_table* table = (t_hash_table*)malloc(sizeof(t_hash_table));
    table->capacity = capacity;
    table->hash_function = &_ht_hash;

    table->slots = (t_hash_table_entry**)calloc(capacity, sizeof(t_hash_table_entry*));
    table->size = 0;
    table->stats = NULL;

    return table;
}

t_hash_table* ht_new_analyze(size_t capacity) {
    t_hash_table* ht = ht_new(capacity);
    ht->stats = calloc(capacity, sizeof(size_t));
}

t_hash_table* ht_new_c_hash(size_t capacity, t_hash_function f) {
    t_hash_table* ht = ht_new(capacity);
    ht->hash_function = f;
    return ht;
}

int ht_insert(t_hash_table* ht, void* key, void* value, size_t key_size, size_t value_size) {

    assert(ht != NULL);
    assert(key != NULL);

    if (_ht_enlarge(ht)) {
        _ht_rehash(ht, ht->capacity * HT_GROWTH_CONST);
        ht->capacity *= HT_GROWTH_CONST;
    }

    size_t hash = ht->hash_function(key, key_size);
    size_t position = (hash) % ht->capacity;


    // If there exists no entry, then set new entry
    if (ht->slots[position] != NULL) {
        // Look for already existing entries
        for (t_hash_table_entry* e = ht->slots[position]; e; e = e->next) {
            if (_ht_key_match(key, key_size, e->key, e->key_size)) {
                e->value = realloc(e->value, value_size);
                memcpy(e->value, value, value_size);
                e->value_size = value_size;

                return HT_UPDATED;
            }
        }
    }

    t_hash_table_entry* entry = malloc(sizeof(t_hash_table_entry));
    entry->key = malloc(sizeof(char) * key_size);
    memcpy(entry->key, key, key_size);
    entry->key_size = key_size;

    entry->value = malloc(sizeof(char) * value_size);
    memcpy(entry->value, value, value_size);
    entry->value_size = value_size;

    entry->next = ht->slots[position];
    entry->previous = NULL;

    if (ht->slots[position] != NULL) {
        ht->slots[position]->previous = entry;
    }

    ht->slots[position] = entry;
    ht->size++;

    if (ht->stats != NULL) {
        ht->stats[position]++;
    }

    return HT_INSERTED;
}

int ht_contains(t_hash_table* ht, void* key, size_t key_size) {
    assert(ht != NULL);
    assert(key != NULL);

    size_t position = (ht->hash_function(key, key_size)) % ht->capacity;
    t_hash_table_entry* entry = ht->slots[position];

    while (entry != NULL) {
        if (_ht_key_match(key, key_size, entry->key, entry->key_size)) {
            return HT_CONTAINS;
        }
        entry = entry->next;
    }

    return HT_NOT_CONTAINS;
}


void* ht_lookup(t_hash_table* ht, void* key, size_t key_size,size_t* value_size) {
    assert(ht != NULL);
    assert(key != NULL);

    size_t position = (ht->hash_function(key, key_size)) % ht->capacity;
    t_hash_table_entry* entry = ht->slots[position];

    while (entry != NULL) {
        if (_ht_key_match(entry->key, entry->key_size, key, key_size)) {
            *value_size = entry->value_size;
            return entry->value;
        }

        entry = entry->next;
    }

    *value_size = 0;
    return NULL;
}

int ht_delete(t_hash_table* ht, void* key, size_t key_size) {
    assert(ht != NULL);
    assert(key != NULL);

    if (_ht_reduce(ht)) {
        _ht_rehash(ht, ht->capacity / HT_SHRINK_CONST);
        ht->capacity /= HT_SHRINK_CONST;
    }

    size_t hash = (ht->hash_function(key, key_size)) % ht->capacity;

    for (t_hash_table_entry* entry = ht->slots[hash]; entry; entry = entry->next) {
        if (_ht_key_match(entry->key, entry->key_size, key, key_size)) {
            t_hash_table_entry* prev = entry->previous;
            t_hash_table_entry* next = entry->next;

            // If previous is NULL then element is the head
            if (prev == NULL) {
                ht->slots[hash] = entry->next;
            } else {
                prev->next = entry->next;
            }

            if (next != NULL && prev != NULL) {
                prev->next = next;
            }

            free(entry->key);
            free(entry->value);
            free(entry);

            if (ht->stats != NULL) {
                ht->stats[hash]--;
            }

            return HT_DELETED;
        }
    }

    return HT_NOT_CONTAINS;
}

void ht_destroy(t_hash_table* ht) {
    for (int i = 0; i < ht->capacity; i++) {
        if (ht->slots[i] != NULL) {
            _ht_destroy_slot(ht->slots[i]);
        }
    }

    if (ht->stats != NULL) {
        free(ht->stats);
    }

    free(ht->slots);
    free(ht);
}

void ht_print_stats(t_hash_table* ht) {
    if (ht->stats != NULL) {
        printf("[ ");

        for (int i = 0; i < ht->capacity - 1; i++) {
            printf(" %ld, ", ht->stats[i]);
        }

        printf(" %ld ]", ht->stats[ht->capacity-1]);
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

void _ht_destroy_slot(t_hash_table_entry* entry) {
    while (entry != NULL) {
        t_hash_table_entry* tmp = entry->next;
        free(entry->key);
        free(entry->value);
        free(entry);
        entry = tmp;
    }
}

int _ht_rehash(t_hash_table* ht, size_t n_capacity) {
    assert(ht != NULL);
    assert(n_capacity > 0);

    t_hash_table_entry** n_slots = (t_hash_table_entry**)calloc(n_capacity, sizeof(t_hash_table_entry*));
    size_t* n_stats;

    if (ht->stats != NULL) {
        n_stats = calloc(n_capacity, sizeof(size_t));
    }

    for (size_t i = 0; i < ht->capacity; i++) {
        for (t_hash_table_entry* entry = ht->slots[i]; entry;) {
            t_hash_table_entry* next = entry->next;

            size_t n_hash = (ht->hash_function(entry->key, entry->key_size)) % n_capacity;

            if (n_slots[n_hash] == NULL) {
                n_slots[n_hash] = entry;
                entry->previous = NULL;
                entry->next = NULL;
            } else {
                n_slots[n_hash]->previous = entry;
                entry->next = n_slots[n_hash];
                n_slots[n_hash] = entry;
            }
            entry = next;
            n_stats[n_hash]++;
        }
    }

    if (ht->stats != NULL) {
        free(ht->stats);
    }

    free(ht->slots);
    ht->slots = n_slots;
    ht->stats = n_stats;

    return HT_SUCCESS;
}

int _ht_enlarge(t_hash_table* ht) {
    return ht->capacity * HT_MAX_LOAD_FACTOR <= ht->size;
}

int _ht_reduce(t_hash_table* ht) {
    return ht->size <= ht->capacity * (1 - HT_MAX_LOAD_FACTOR);
}