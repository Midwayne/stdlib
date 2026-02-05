#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>

typedef size_t hash_t;

/**
 * Generic Hash Function (FNV-1a Algorithm)
 * Hashes any block of memory of a given size.
 *
 * @param data Pointer to the data to hash
 * @param size Number of bytes to read
 * @return The calculated hash code
 */
hash_t hash_bytes(const void* data, size_t size);

// Helpers for common types

// Hashes a null-terminated C string
hash_t hash_string(const void* data);

// Hashes a standard integer
hash_t hash_int(const void* data);

// Hashes a long integer
hash_t hash_long(const void* data);

// Hashes a pointer address itself
hash_t hash_ptr(const void* data);

#endif
