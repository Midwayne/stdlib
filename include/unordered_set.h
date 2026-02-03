#ifndef UNORDERED_SET_H
#define UNORDERED_SET_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Hash function type.
 * Should return a generic hash code the given key.
 */
typedef size_t (*HashFunc)(const void *key);

/**
 * Key comparison function type.
 * Should return true if key1 equals key2, false otherwise.
 */
typedef bool (*KeyCompare)(const void *key1, const void *key2);

// Internal linked list node for separate chaining
typedef struct SetNode {
	// Pointer to the heap allocated key copy
	void *key;

	// Pointer to the next node in the bucket
	struct SetNode *next;
} SetNode;

// Unordered set structure
typedef struct {
	// Dyanamic array of bucket pointers
	SetNode **buckets;
	// Current capacity, i.e. the number of buckets
	size_t bucket_count;
	// Total number of elements stored
	size_t size;

	// Size of the key in bytes
	size_t key_size;

	HashFunc hash_func;
	KeyCompare key_compare;
} UnorderedSet;


// Lifecycle management

/**
 * Initialize the set.
 * @param set Pointer to the UnorderedSet to initialize
 * @param key_size Size of the key type in bytes
 * @param hf Hashing function for the key type
 * @param cmp Comparison function for the key type
 */
void unordered_set_init(UnorderedSet *set, size_t key_size, HashFunc hf, KeyCompare cmp);

/**
 * Frees all memory associated with the set (keys, nodes and buckets).
 * @param set Pointer to the UnorderedSet to free
 */
void unordered_set_free(UnorderedSet *set);


// Modifiers
//
/**
 * Inserts a key into the set.
 * Creates a copy of the key.
 * @return true if the key was inserted, false if it already existed.
 */
bool unordered_set_insert(UnorderedSet *set, const void *key);

/**
 * Removes a key from the set if it exists.
 * @param key Pointer to the key to remove
 */
void unordered_set_erase(UnorderedSet *set, const void *key);

/**
 * Removes all elements from the set but keeps the bucket capacity.
 */
void unordered_set_clear(UnorderedSet *set);


// Access

/**
 * Checks if a key exists in the set.
 * @return true if found, false otherwise.
 */
bool unordered_set_contains(UnorderedSet *set, const void *key);

/**
 * Returns the number of elements in the set.
 */
static inline size_t unordered_set_size(UnorderedSet *set) {
    return set->size;
}

/**
 * Checks if the set is empty.
 */
static inline bool unordered_set_is_empty(UnorderedSet *set) {
    return set->size == 0;
}

#endif

