#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

#define VECTOR_ASSERT(condition, message) \
		if(!(condition)) { \
			fprintf(stderr, "Error: [%s:%d] %s\n", __FILE__, __LINE__, message); \
			exit(1); \
		} 

const int DEFAULT_VECTOR_CAPACITY = 20;

void vector_init(Vector *v, size_t element_size) {
	VECTOR_ASSERT(element_size > 0, "cannot initialise a vector with element size less than 1");

	v->capacity = DEFAULT_VECTOR_CAPACITY;
	v->element_size = element_size;
	v->size = 0;
	
	v->items = (void *)malloc(v->capacity * v->element_size);
	VECTOR_ASSERT(v->items != NULL, "failed to allocate memory for vector");
}

void vector_reserve(Vector *v, size_t capacity) {
	if(capacity < v->capacity) return;

	void *ptr = (void *)realloc(v->items, v->capacity * v->element_size);

	VECTOR_ASSERT(ptr != NULL, "failed to reallocate memory");

	v->items = ptr;
	v->capacity = capacity;
}

void vector_free(Vector *v) {
	if(v == NULL) return;

	if(v->items != NULL) {
		free(v->items);
		v->items = NULL;
	}

	v->size = 0;
	v->capacity = 0;
}

