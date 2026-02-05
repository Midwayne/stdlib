#include "unordered_set.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_BUCKET_COUNT 16
#define LOAD_FACTOR_THRESHOLD 0.75

#ifndef UNORDERED_SET_ASSERT
#define UNORDERED_SET_ASSERT(condition, message)                             \
    if (!(condition)) {                                                      \
        fprintf(stderr, "Error: [%s:%d] %s\n", __FILE__, __LINE__, message); \
        exit(1);                                                             \
    }
#endif

/* ============================================================
 * Lifecycle
 * ============================================================
 */

void unordered_set_init(UnorderedSet* set, size_t key_size, HashFunc hf, KeyCompare cmp) {
    UNORDERED_SET_ASSERT(set != NULL, "set pointer cannot be NULL");
    UNORDERED_SET_ASSERT(key_size > 0, "key size must be > 0");
    UNORDERED_SET_ASSERT(hf != NULL, "hash function cannot be NULL");
    UNORDERED_SET_ASSERT(cmp != NULL, "comparison function cannot be NULL");

    set->key_size = key_size;
    set->hash_func = hf;
    set->key_compare = cmp;
    set->size = 0;
    set->bucket_count = DEFAULT_BUCKET_COUNT;

    set->buckets = (SetNode**)calloc(set->bucket_count, sizeof(SetNode*));
    UNORDERED_SET_ASSERT(!(set->buckets == NULL), "failed to allocate memory");
}

void unordered_set_free(UnorderedSet* set) {
    if (set == NULL) return;

    if (set->buckets != NULL) {
        for (size_t i = 0; i < set->bucket_count; i++) {
            SetNode* current = set->buckets[i];
            while (current != NULL) {
                SetNode* next = current->next;
                if (current->key) free(current->key);
                free(current);
                current = next;
            }
        }
        free(set->buckets);
        set->buckets = NULL;
    }
    set->size = 0;
    set->bucket_count = 0;
}

/* ============================================================
 * Modifiers
 * ============================================================
 */

/*
 * Resizes the hash table when load factor is exceeded.
 * Doubles capacity and redistributes existing nodes.
 */
static void unordered_set_rehash(UnorderedSet* set, size_t new_capacity) {
    SetNode** new_buckets = (SetNode**)calloc(new_capacity, sizeof(SetNode*));
    UNORDERED_SET_ASSERT(new_buckets != NULL, "failed to allocate memory for resize");

    for (size_t i = 0; i < set->bucket_count; i++) {
        SetNode* current = set->buckets[i];

        while (current != NULL) {
            SetNode* next = current->next;
            size_t hash = set->hash_func(current->key);
            size_t new_idx = hash % new_capacity;

            current->next = new_buckets[new_idx];
            new_buckets[new_idx] = current;

            current = next;
        }
    }

    free(set->buckets);
    set->buckets = new_buckets;
    set->bucket_count = new_capacity;
}

bool unordered_set_insert(UnorderedSet* set, const void* key) {
    UNORDERED_SET_ASSERT(set != NULL, "set pointer cannot be NULL");
    UNORDERED_SET_ASSERT(key != NULL, "cannot insert NULL key");

    // If (size + 1) / bucket_count > 0.75, we double the size.
    if ((double)(set->size + 1) / set->bucket_count > LOAD_FACTOR_THRESHOLD) {
        unordered_set_rehash(set, set->bucket_count * 2);
    }

    size_t hash = set->hash_func(key);
    size_t index = hash % set->bucket_count;

    SetNode* current = set->buckets[index];
    while (current != NULL) {
        if (set->key_compare(current->key, key)) {
            // Key already exists
            return false;
        }
        current = current->next;
    }

    SetNode* new_node = (SetNode*)malloc(sizeof(SetNode));
    UNORDERED_SET_ASSERT(new_node != NULL, "failed to allocate new node");

    // Allocate memory for the key copy (deep copy)
    new_node->key = malloc(set->key_size);
    UNORDERED_SET_ASSERT(new_node->key != NULL, "failed to allocate key memory");

    memcpy(new_node->key, key, set->key_size);

    // inserting at head of bucket since this is O(1)
    new_node->next = set->buckets[index];
    set->buckets[index] = new_node;
    set->size++;

    return true;
}

/* ============================================================
 * Access
 * ============================================================
 */

// TODO
bool unordered_set_contains(UnorderedSet* set, const void* key) {
    fprintf(stdout, "%zu", set->size);
    fprintf(stdout, "%p", key);
    return true;
}
