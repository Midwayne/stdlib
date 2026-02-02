#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "hash.h"

/*
 * FNV-1a implementation that adapts to size_t (32 or 64-bit).
 * Returns a value of type hash_t
 */

#if SIZE_MAX == UINT64_MAX
	/* 64-bit */
	static const uint64_t FNV_OFFSET_BASIS_64 = 14695981039346656037ULL;
	static const uint64_t FNV_PRIME_64        = 1099511628211ULL;
#elif SIZE_MAX == UINT32_MAX
	/* 32-bit */
	static const uint32_t FNV_OFFSET_BASIS_32 = 2166136261u;
	static const uint32_t FNV_PRIME_32        = 16777619u;
#else
	#error "Unsupported size_t width for FNV implementation"
#endif

hash_t hash_bytes(const void *data, size_t size) {
    if (size == 0) {
#if SIZE_MAX == UINT64_MAX
        return (hash_t)FNV_OFFSET_BASIS_64;
#else
        return (hash_t)FNV_OFFSET_BASIS_32;
#endif
    }

    if (data == NULL) {
        return (hash_t)0;
    }

#if SIZE_MAX == UINT64_MAX
    uint64_t h = FNV_OFFSET_BASIS_64;
    const unsigned char *p = (const unsigned char *)data;
    for (size_t i = 0; i < size; ++i) {
        h ^= (uint64_t)p[i];
        h *= FNV_PRIME_64;
    }
    return (hash_t)h;
#else
    uint32_t h = FNV_OFFSET_BASIS_32;
    const unsigned char *p = (const unsigned char *)data;
    for (size_t i = 0; i < size; ++i) {
        h ^= (uint32_t)p[i];
        h *= FNV_PRIME_32;
    }
    return (hash_t)h;
#endif
}

hash_t hash_string(const void *data) {
	if (data == NULL) return (hash_t)0;
	const char *s = (const char *)data;
	return hash_bytes(s, strlen(s)); // do not include terminating \0
}

hash_t hash_int(const void *data) {
	if (data == NULL) return (hash_t)0;
	return hash_bytes(data, sizeof(int));
}

hash_t hash_long(const void *data) {
	if (data == NULL) return (hash_t)0;
	return hash_bytes(data, sizeof(long));
}

hash_t hash_ptr(const void *data) {
	if (data == NULL) return (hash_t)0;
	uintptr_t val = (uintptr_t)data;
	return hash_bytes(&val, sizeof(val));
}

