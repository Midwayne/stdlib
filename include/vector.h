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

/*
 * Initialise a vector with default capacity
 * @param v Pointer to the Vector to initialise
 * @param element_size Size of each element in bytes
 */
void vector_init(Vector *v, size_t element_size);

/*
 * Reserve a specific amount of memory for the vector (should be used after initialisation)
 * @param v Pointer to the Vector
 * @param capacity Number of elements to reserve space for
 */
void vector_reserve(Vector *v, size_t capacity);

/*
 * Free memory associated with the vector
 * @param v Pointer to the Vector to free
 */
void vector_free(Vector *v);



/* Access */

/*
 * Gets the element in the vector at the required index
 * @param v Pointer to the Vector
 * @param index Index of the element to retrieve
 */
void* vector_get(Vector *v, size_t index);

/*
 * Returns a pointer to the last element
 * @param v Pointer to the Vector
 */
void* vector_front(Vector *v);

/*
 * Returns a pointer to the first element
 * @param v Pointer to the Vector
 */
void* vector_back(Vector *v);



/* Modifiers */

/*
 * Assigns the element in the vector at the given index (overwrites the previous element)
 * @param v Pointer to the Vector
 * @param index Index where the element will be set
 * @param element Pointer to the element to copy into the vector
 */
void vector_set(Vector *v, size_t index, const void* element);

/*
 * Assigns the element to the end of the vector
 * @param v Pointer to the Vector
 * @param element Pointer to the element to push back
 */
void vector_push_back(Vector *v, const void* element);

/*
 * Decrements the size of the vector (It will not free the memory)
 * @param v Pointer to the Vector
 */
void vector_pop_back(Vector *v);

/*
 * Assigns the element in the vector at the given index (shifts all the existing elements to the right)
 * @param v Pointer to the Vector
 * @param index Index where the element will be inserted
 * @param element Pointer to the element to insert
 */
void vector_insert(Vector *v, size_t index, const void* element);

/*
 * Removes the element at the given index (shifts all the existing elements to the left)
 * @param v Pointer to the Vector
 * @param index Index of the element to remove
 */
void vector_erase(Vector *v, size_t index);

/*
 * Resets the vector size to 0 while keeping the existing capacity
 * @param v Pointer to the Vector
 */
void vector_clear(Vector *v);

/*
 * Shrinks the vector capacity to make the current size
 * @param v Pointer to the Vector
 */
void vector_shrink_to_fit(Vector *v);



/* Status */

/*
 * Returns the current size of the vector
 * @param v Pointer to the Vector
 */
size_t vector_size(Vector *v);

/*
 * Returns 1 if the vector is empty, else returns 1
 * @param v Pointer to the Vector
 */
bool vector_is_empty(Vector *v);

#endif

