#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <check.h>

#include "hash.h"


#if defined(__GNUC__) || defined(__clang__)
#  define UNUSED_ATTR __attribute__((unused))
#else
#  define UNUSED_ATTR
#endif

/* ============================================================
 * Reference FNV-1a (canonical) implementations
 * ============================================================ 
 */

static uint32_t ref_fnv1a_32(const void *data, size_t size) UNUSED_ATTR;
static uint64_t ref_fnv1a_64(const void *data, size_t size) UNUSED_ATTR;

static uint32_t ref_fnv1a_32(const void *data, size_t size)
{
	const uint8_t *p = (const uint8_t *)data;
	uint32_t hash = 2166136261u;

	for (size_t i = 0; i < size; ++i) {
		hash ^= p[i];
		hash *= 16777619u;
	}
	return hash;
}

static uint64_t ref_fnv1a_64(const void *data, size_t size)
{
	const uint8_t *p = (const uint8_t *)data;
	uint64_t hash = 14695981039346656037ULL;

	for (size_t i = 0; i < size; ++i) {
		hash ^= p[i];
		hash *= 1099511628211ULL;
	}
	return hash;
}

/* ============================================================
 * Tests for hash_bytes
 * ============================================================ 
 */

START_TEST(test_hash_bytes_basic)
{
	const char *s = "hello";

#if SIZE_MAX == UINT64_MAX
	ck_assert_uint_eq(
		hash_bytes(s, strlen(s)),
		(hash_t)ref_fnv1a_64(s, strlen(s))
	);
#else
	ck_assert_uint_eq(
		hash_bytes(s, strlen(s)),
		(hash_t)ref_fnv1a_32(s, strlen(s))
	);
#endif
}
END_TEST

START_TEST(test_hash_bytes_binary)
{
	uint8_t data[] = {0x00, 0x01, 0x02, 0xff};

#if SIZE_MAX == UINT64_MAX
	ck_assert_uint_eq(
		hash_bytes(data, sizeof(data)),
		(hash_t)ref_fnv1a_64(data, sizeof(data))
	);
#else
	ck_assert_uint_eq(
		hash_bytes(data, sizeof(data)),
		(hash_t)ref_fnv1a_32(data, sizeof(data))
	);
#endif
}
END_TEST

START_TEST(test_hash_bytes_empty)
{
	uint8_t dummy = 0;

#if SIZE_MAX == UINT64_MAX
	ck_assert_uint_eq(
		hash_bytes(&dummy, 0),
		(hash_t)ref_fnv1a_64(&dummy, 0)
	);
#else
	ck_assert_uint_eq(
		hash_bytes(&dummy, 0),
		(hash_t)ref_fnv1a_32(&dummy, 0)
	);
#endif
}
END_TEST

START_TEST(test_hash_bytes_32_bytes)
{
	uint8_t data[32];

	for (size_t i = 0; i < sizeof(data); ++i) {
		data[i] = (uint8_t)i;
	}

#if SIZE_MAX == UINT64_MAX
	ck_assert_uint_eq(
		hash_bytes(data, sizeof(data)),
		(hash_t)ref_fnv1a_64(data, sizeof(data))
	);
#else
	ck_assert_uint_eq(
		hash_bytes(data, sizeof(data)),
		(hash_t)ref_fnv1a_32(data, sizeof(data))
	);
#endif
}
END_TEST

/* ============================================================
 * Tests for helper functions
 * ============================================================ 
 */

START_TEST(test_hash_string)
{
	const char *s = "a random string, can we get this working?";

	ck_assert_uint_eq(
		hash_string(s),
		hash_bytes(s, strlen(s))
	);
}
END_TEST

START_TEST(test_hash_int)
{
	int x = 0x12345678;

	ck_assert_uint_eq(
		hash_int(&x),
		hash_bytes(&x, sizeof(x))
	);
}
END_TEST

START_TEST(test_hash_long)
{
	long x = 0x123456789abcdefL;

	ck_assert_uint_eq(
		hash_long(&x),
		hash_bytes(&x, sizeof(x))
	);
}
END_TEST

START_TEST(test_hash_ptr)
{
	int x;
	uintptr_t p = (uintptr_t)&x;

	ck_assert_uint_eq(
		hash_ptr(&x),
		hash_bytes(&p, sizeof(p))
	);
}
END_TEST

/* ============================================================
 * Test suite
 * ============================================================
 */

Suite *hash_suite(void)
{
	Suite *s;
	TCase *tc_bytes, *tc_helpers;

	s = suite_create("Hash");

	tc_bytes = tcase_create("Bytes");
	tcase_add_test(tc_bytes, test_hash_bytes_basic);
	tcase_add_test(tc_bytes, test_hash_bytes_binary);
	tcase_add_test(tc_bytes, test_hash_bytes_empty);
	tcase_add_test(tc_bytes, test_hash_bytes_32_bytes);
	suite_add_tcase(s, tc_bytes);

	tc_helpers = tcase_create("Helpers");
	tcase_add_test(tc_helpers, test_hash_string);
	tcase_add_test(tc_helpers, test_hash_int);
	tcase_add_test(tc_helpers, test_hash_long);
	tcase_add_test(tc_helpers, test_hash_ptr);
	suite_add_tcase(s, tc_helpers);

	return s;
}
