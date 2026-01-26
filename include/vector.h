#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
	// Pointer to the dynamically allocated array
	void *items;
	// The size of a single element in the vector (in bytes)
	size_t element_size;
	// Number of elements currently in the vector
	size_t size;
	// Total number of elements the current memory block can hold
	size_t capacity;
} Vector;



/* Lifecycle management */

// Initialise a vector with default capacity
void vector_init(Vector *v, size_t element_size);

// Reserve a specific amount of memory for the vector (should be used after initialisation)
void vector_reserve(Vector *v, size_t capacity);

// Free memory associated with the vector
void vector_free(Vector *v);



/* Access */

// Gets the element in the vector at the required index
void* vector_get(Vector *v, size_t index);

// Returns a pointer to the last element
void* vector_front(Vector *v);

// Returns a pointer to the first element
void* vector_back(Vector *v);



/* Modifiers */

// Assigns the element in the vector at the given index (overwrites the previous element)
void vector_set(Vector *v, size_t index, const void* element);

// Assigns the element to the end of the vector
void vector_push_back(Vector *v, const void* element);

// Decrements the size of the vector (It will not free the memory)
void vector_pop_back(Vector *v);

// Assigns the element in the vector at the given index (shifts all the existing elements to the right)
void vector_insert(Vector *v, size_t index, const void* element);

// Removes the element at the given index (shifts all the existing elements to the left)
void vector_erase(Vector *v, size_t index);

// Resets the vector size to 0 while keeping the existing capacity
void vector_clear(Vector *v);

// Shrinks the vector capacity to make the current size
void vector_shrink_to_fit(Vector *v);



/* Status */

// Returns the current size of the vector
size_t vector_size(Vector *v);

// Returns 1 if the vector is empty, else returns 1
bool vector_is_empty(Vector *v);

#endif

