#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "unordered_set.h"

#define DEFAULT_BUCKET_COUNT 16

#ifndef UNORDERED_SET_ASSERT
#define UNORDERED_SET_ASSERT(condition, message) \
		if(!(condition)) { \
			fprintf(stderr, "Error: [%s:%d] %s\n", __FILE__, __LINE__, message); \
			exit(1); \
		} 
#endif


/* ============================================================
 * Lifecycle
 * ============================================================
 */

void unordered_set_init(UnorderedSet *set, size_t key_size, HashFunc hf, KeyCompare cmp) {
	UNORDERED_SET_ASSERT(set != NULL, "set pointer cannot be NULL");
    UNORDERED_SET_ASSERT(key_size > 0, "key size must be > 0");
    UNORDERED_SET_ASSERT(hf != NULL, "hash function cannot be NULL");
    UNORDERED_SET_ASSERT(cmp != NULL, "comparison function cannot be NULL");
	
	set->key_size = key_size;
	set->hash_func = hf;
	set->key_compare = cmp;
	set->size = 0;
	set->bucket_count = DEFAULT_BUCKET_COUNT;

	set->buckets = (SetNode **)calloc(set->bucket_count, sizeof(SetNode*));
	UNORDERED_SET_ASSERT(!(set->buckets == NULL), "failed to allocate memory");
}

// TODO: full node cleanup impl. This is a placeholder to pass the existing tests
void unordered_set_free(UnorderedSet *set) {
    if (set->buckets != NULL) {
        free(set->buckets);
        set->buckets = NULL;
    }
    set->size = 0;
    set->bucket_count = 0;
}

