#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

#ifndef VECTOR_ASSERT
#define VECTOR_ASSERT(condition, message) \
		if(!(condition)) { \
			fprintf(stderr, "Error: [%s:%d] %s\n", __FILE__, __LINE__, message); \
			exit(1); \
		} 
#endif

const int DEFAULT_VECTOR_CAPACITY = 20;

// =========== LIFECYCLE MANAGEMENT ===========

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

	void *ptr = (void *)realloc(v->items, capacity * v->element_size);

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

// =========== ACCESS ===========

void* vector_get(Vector *v, size_t index) {
	VECTOR_ASSERT(index < v->size, "index out of bounds");
	return (unsigned char *)v->items + (v->element_size * index);
} 

void* vector_front(Vector *v) {
	return vector_get(v, 0);
}

void* vector_back(Vector *v) {
	return vector_get(v, v->size-1);
} 

// =========== MODIFIERS ===========

void vector_set(Vector *v, size_t index, const void *element) {
	VECTOR_ASSERT(index < v->size, "index out of bounds");
	VECTOR_ASSERT(element != NULL, "invalid element pointer");

	void *ptr = (unsigned char *)v->items + (v->element_size*index);

	memcpy(ptr, element, v->element_size);
}

void vector_push_back(Vector *v, const void *element) {
	VECTOR_ASSERT(element != NULL, "invalid element pointer");

	if (v->capacity == 0) vector_reserve(v, DEFAULT_VECTOR_CAPACITY);
	else if (v->size == v->capacity) vector_reserve(v, v->capacity * 2);

	void *ptr = (unsigned char *)v->items + (v->element_size*v->size);

	memcpy(ptr, element, v->element_size);
	++v->size;
}

void vector_pop_back(Vector *v) {
	VECTOR_ASSERT(v->size != 0, "no elements in the vector");

	--v->size;
}

void vector_insert(Vector *v, size_t index, const void *element) {
	VECTOR_ASSERT(index <= v->size, "index out of bounds");
	VECTOR_ASSERT(element != NULL, "invalid element pointer");

	if (v->capacity == 0) vector_reserve(v, DEFAULT_VECTOR_CAPACITY);
	else if (v->size == v->capacity) vector_reserve(v, v->capacity * 2);

	void *ptr = (unsigned char *)v->items + (v->element_size*index);

	if(index < v->size) memmove((unsigned char *)ptr+v->element_size, ptr, (v->size - index)*v->element_size);
	memcpy(ptr, element, v->element_size);
	++v->size;
}

void vector_erase(Vector *v, size_t index) {
	VECTOR_ASSERT(index < v->size, "index out of bounds");

	void *ptr = (unsigned char *)v->items + (v->element_size*index);
	if (index < v->size-1) memmove(ptr, (unsigned char *)ptr+v->element_size, (v->size - index - 1)*v->element_size);
	--v->size;
}

void vector_clear(Vector *v) {
	// VECTOR_ASSERT(v->size != 0, "no elements in the vector");
	 v->size = 0;
}

void vector_shrink_to_fit(Vector *v) {
	if (v->size == v->capacity || v->capacity == 0) return;

	if (v->size == 0) {
		free(v->items);
		v->items = NULL;
		v->capacity = 0;
		return;
	}

	void *ptr = realloc(v->items, v->size * v->element_size);
	
	if (ptr != NULL) {
		v->items = ptr;
		v->capacity = v->size;
	}
}

// =========== STATUS ===========

size_t vector_size(Vector *v) {
	return v->size;
}

bool vector_is_empty(Vector *v) {
	return v->size == 0;
}

