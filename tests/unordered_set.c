#include <check.h>
#include <stdint.h>

#include "hash.h"
#include "unordered_set.h"

#define DEFAULT_BUCKET_COUNT 16

bool int_compare(const void *a, const void *b) {
    return *(const int*)a == *(const int*)b;
}

/* ============================================================
 * Lifecycle
 * ============================================================
 */

START_TEST(test_unordered_set_init)
{
	UnorderedSet test_set;
	unordered_set_init(&test_set, sizeof(uint16_t), hash_int, int_compare);

	ck_assert_uint_eq(test_set.size, 0);
	ck_assert_uint_eq(test_set.key_size, sizeof(uint16_t));
	ck_assert_ptr_nonnull(test_set.buckets);

    ck_assert_uint_eq(test_set.bucket_count, DEFAULT_BUCKET_COUNT); 

    unordered_set_free(&test_set);
}
END_TEST


/* ============================================================
 * Test suite
 * ============================================================
 */

Suite *unordered_set_suite(void)
{
	Suite *s;
	TCase *tc_lifecycle;

	s = suite_create("UNORDERED_SET");

	/* Lifecycle test case */
	tc_lifecycle = tcase_create("Lifecycle");
	tcase_add_test(tc_lifecycle, test_unordered_set_init);
	suite_add_tcase(s, tc_lifecycle);

	return s;
}

