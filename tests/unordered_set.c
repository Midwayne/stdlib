#include "unordered_set.h"

#include <check.h>
#include <stdint.h>

#include "hash.h"

#define DEFAULT_BUCKET_COUNT 16

bool int_compare(const void* a, const void* b) { return *(const int*)a == *(const int*)b; }

/* ============================================================
 * Lifecycle
 * ============================================================
 */

START_TEST(test_unordered_set_init) {
    UnorderedSet test_set;
    unordered_set_init(&test_set, sizeof(int), hash_int, int_compare);

    ck_assert_uint_eq(test_set.size, 0);
    ck_assert_uint_eq(test_set.key_size, sizeof(int));
    ck_assert_ptr_nonnull(test_set.buckets);

    ck_assert_uint_eq(test_set.bucket_count, DEFAULT_BUCKET_COUNT);

    unordered_set_free(&test_set);
}
END_TEST

/* ============================================================
 * Modifiers
 * ============================================================
 */

START_TEST(test_unordered_set_insert) {
    UnorderedSet test_set;
    unordered_set_init(&test_set, sizeof(int), hash_int, int_compare);

    int val1 = 10;
    int val2 = 20;

    ck_assert_msg(unordered_set_insert(&test_set, &val1) == true,
                  "First insert should return true");
    ck_assert_uint_eq(test_set.size, 1);

    ck_assert_msg(unordered_set_insert(&test_set, &val2) == true,
                  "Second insert should return true");
    ck_assert_uint_eq(test_set.size, 2);

    ck_assert_msg(unordered_set_insert(&test_set, &val1) == false,
                  "Duplicate insert should return false");
    ck_assert_uint_eq(test_set.size, 2);

    unordered_set_free(&test_set);
}
END_TEST

START_TEST(test_unordered_set_resize) {
    UnorderedSet test_set;
    unordered_set_init(&test_set, sizeof(int), hash_int, int_compare);

    int limit = 25;
    for (int i = 0; i < limit; i++) {
        unordered_set_insert(&test_set, &i);
    }

    // Capacity starts at 16.
    // > 12 items -> Resize to 32
    // > 24 items -> Resize to 64
    // So with 25 items, we expect 64 buckets
    ck_assert_uint_eq(test_set.bucket_count, 64);

    ck_assert_uint_eq(test_set.size, limit);

    for (int i = 0; i < limit; i++) {
        ck_assert_msg(unordered_set_contains(&test_set, &i), "List element %d during resize", i);
    }

    unordered_set_free(&test_set);
}
END_TEST

size_t bad_hash(const void* key) {
    (void)key;
    return 0;  // Everything goes to Bucket[0]
}

START_TEST(test_unordered_set_erase) {
    UnorderedSet set;
    // bad_hash to force everything into one bucket
    unordered_set_init(&set, sizeof(int), bad_hash, int_compare);

    int v1 = 1, v2 = 2, v3 = 3;

    unordered_set_insert(&set, &v1);
    unordered_set_insert(&set, &v2);
    unordered_set_insert(&set, &v3);

    // Current State (Insert at Head): 3 -> 2 -> 1 -> NULL
    ck_assert_uint_eq(set.size, 3);

    // Case 1: Erase Middle (v2)
    // Expected: 3 -> 1 -> NULL
    unordered_set_erase(&set, &v2);
    ck_assert(!unordered_set_contains(&set, &v2));
    ck_assert(unordered_set_contains(&set, &v3));
    ck_assert(unordered_set_contains(&set, &v1));
    ck_assert_uint_eq(set.size, 2);

    // Case 2: Erase Head (v3)
    // Expected: 1 -> NULL
    unordered_set_erase(&set, &v3);
    ck_assert(!unordered_set_contains(&set, &v3));
    ck_assert(unordered_set_contains(&set, &v1));
    ck_assert_uint_eq(set.size, 1);

    // Case 3: Erase Tail (v1)
    // Expected: Empty
    unordered_set_erase(&set, &v1);
    ck_assert_uint_eq(set.size, 0);
    ck_assert(unordered_set_is_empty(&set));

    unordered_set_free(&set);
}
END_TEST

START_TEST(test_unordered_set_clear) {
    UnorderedSet set;
    unordered_set_init(&set, sizeof(int), hash_int, int_compare);

    int v1 = 10, v2 = 20, v3 = 30;
    unordered_set_insert(&set, &v1);
    unordered_set_insert(&set, &v2);
    unordered_set_insert(&set, &v3);

    ck_assert_uint_eq(set.size, 3);

    unordered_set_clear(&set);

    ck_assert_uint_eq(set.size, 0);
    ck_assert(unordered_set_is_empty(&set));
    ck_assert(!unordered_set_contains(&set, &v1));

    unordered_set_insert(&set, &v1);
    ck_assert_uint_eq(set.size, 1);
    ck_assert(unordered_set_contains(&set, &v1));

    unordered_set_free(&set);
}
END_TEST

/* ============================================================
 * Access suite
 * ============================================================
 */

START_TEST(test_unordered_set_contains) {
    UnorderedSet set;
    unordered_set_init(&set, sizeof(int), hash_int, int_compare);

    int val = 42;
    int missing = 99;

    ck_assert_msg(unordered_set_contains(&set, &val) == false, "Empty set should not find value");

    unordered_set_insert(&set, &val);

    ck_assert_msg(unordered_set_contains(&set, &val) == true, "Should contain inserted value");
    ck_assert_msg(unordered_set_contains(&set, &missing) == false,
                  "Should not contain missing value");

    unordered_set_free(&set);
}
END_TEST

/* ============================================================
 * Test suite
 * ============================================================
 */

Suite* unordered_set_suite(void) {
    Suite* s;
    TCase *tc_lifecycle, *tc_modifiers, *tc_access;

    s = suite_create("UNORDERED_SET");

    /* Lifecycle test case */
    tc_lifecycle = tcase_create("Lifecycle");
    tcase_add_test(tc_lifecycle, test_unordered_set_init);
    suite_add_tcase(s, tc_lifecycle);

    /* Modifiers test case */
    tc_modifiers = tcase_create("Modifiers");
    tcase_add_test(tc_modifiers, test_unordered_set_insert);
    tcase_add_test(tc_modifiers, test_unordered_set_resize);
    tcase_add_test(tc_modifiers, test_unordered_set_erase);
    tcase_add_test(tc_modifiers, test_unordered_set_clear);
    suite_add_tcase(s, tc_modifiers);

    /* Access test case */
    tc_access = tcase_create("Access");
    tcase_add_test(tc_access, test_unordered_set_contains);

    return s;
}
