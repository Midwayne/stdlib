#include "vector.h"

#include <check.h>
#include <stdlib.h>

#define TEST_DEFAULT_CAPACITY 20

/* =========== LIFECYCLE MANAGEMENT =========== */

START_TEST(test_vector_init) {
    Vector v;
    vector_init(&v, sizeof(int));

    ck_assert_uint_eq(v.size, 0);
    ck_assert_uint_eq(v.element_size, sizeof(int));
    ck_assert_uint_eq(v.capacity, TEST_DEFAULT_CAPACITY);
    ck_assert_ptr_nonnull(v.items);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_reserve_smaller) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_reserve(&v, 5);
    ck_assert_uint_eq(v.capacity, TEST_DEFAULT_CAPACITY);
    ck_assert_ptr_nonnull(v.items);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_reserve_larger) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_reserve(&v, 50);
    ck_assert_uint_eq(v.capacity, 50);
    ck_assert_ptr_nonnull(v.items);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_free) {
    Vector v;
    vector_init(&v, sizeof(int));
    vector_free(&v);

    ck_assert_uint_eq(v.capacity, 0);
    ck_assert_uint_eq(v.size, 0);
    ck_assert_ptr_null(v.items);
}
END_TEST

/* =========== ACCESS =========== */

START_TEST(test_vector_get) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_push_back(&v, &(int){1});
    vector_push_back(&v, &(int){2});

    ck_assert_int_eq(*(int*)vector_get(&v, 0), 1);
    ck_assert_int_eq(*(int*)vector_get(&v, 1), 2);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_get_crash) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_get(&v, 5);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_front_back) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_push_back(&v, &(int){10});
    vector_push_back(&v, &(int){20});
    vector_push_back(&v, &(int){30});

    ck_assert_int_eq(*(int*)vector_front(&v), 10);
    ck_assert_int_eq(*(int*)vector_back(&v), 30);

    vector_free(&v);
}
END_TEST

/* =========== MODIFIERS =========== */

START_TEST(test_vector_set) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_push_back(&v, &(int){1});
    vector_set(&v, 0, &(int){100});

    ck_assert_int_eq(*(int*)vector_get(&v, 0), 100);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_set_crash) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_set(&v, 5, &(int){50});

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_push_back) {
    Vector v;
    vector_init(&v, sizeof(int));

    for (int i = 0; i < 25; i++) {
        vector_push_back(&v, &i);
    }

    ck_assert_uint_eq(v.size, 25);
    ck_assert_uint_ge(v.capacity, 25);
    ck_assert_int_eq(*(int*)vector_get(&v, 24), 24);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_pop_back) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_push_back(&v, &(int){1});
    vector_push_back(&v, &(int){2});
    vector_pop_back(&v);

    ck_assert_uint_eq(v.size, 1);
    ck_assert_int_eq(*(int*)vector_get(&v, 0), 1);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_pop_back_crash) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_pop_back(&v);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_insert) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_push_back(&v, &(int){1});
    vector_push_back(&v, &(int){3});
    vector_insert(&v, 1, &(int){2});

    ck_assert_uint_eq(v.size, 3);
    ck_assert_int_eq(*(int*)vector_get(&v, 0), 1);
    ck_assert_int_eq(*(int*)vector_get(&v, 1), 2);
    ck_assert_int_eq(*(int*)vector_get(&v, 2), 3);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_erase) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_push_back(&v, &(int){1});
    vector_push_back(&v, &(int){2});
    vector_push_back(&v, &(int){3});
    vector_erase(&v, 1);

    ck_assert_uint_eq(v.size, 2);
    ck_assert_int_eq(*(int*)vector_get(&v, 0), 1);
    ck_assert_int_eq(*(int*)vector_get(&v, 1), 3);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_clear) {
    Vector v;
    vector_init(&v, sizeof(int));

    vector_push_back(&v, &(int){1});
    vector_push_back(&v, &(int){2});
    vector_clear(&v);

    ck_assert_uint_eq(v.size, 0);
    ck_assert_uint_ge(v.capacity, TEST_DEFAULT_CAPACITY);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_shrink_to_fit) {
    Vector v;
    vector_init(&v, sizeof(int));

    for (int i = 0; i < 5; i++) {
        vector_push_back(&v, &i);
    }

    vector_shrink_to_fit(&v);

    ck_assert_uint_eq(v.size, 5);
    ck_assert_uint_eq(v.capacity, 5);

    vector_free(&v);
}
END_TEST

/* =========== STATUS =========== */

START_TEST(test_vector_size) {
    Vector v;
    vector_init(&v, sizeof(int));

    ck_assert_uint_eq(vector_size(&v), 0);

    vector_push_back(&v, &(int){1});
    ck_assert_uint_eq(vector_size(&v), 1);

    vector_free(&v);
}
END_TEST

START_TEST(test_vector_is_empty) {
    Vector v;
    vector_init(&v, sizeof(int));

    ck_assert(vector_is_empty(&v));

    vector_push_back(&v, &(int){1});
    ck_assert(!vector_is_empty(&v));

    vector_free(&v);
}
END_TEST

Suite* vector_suite(void) {
    Suite* s;
    TCase *tc_lifecycle, *tc_access, *tc_modifiers, *tc_status;

    s = suite_create("Vector");

    /* Lifecycle test case */
    tc_lifecycle = tcase_create("Lifecycle");
    tcase_add_test(tc_lifecycle, test_vector_init);
    tcase_add_test(tc_lifecycle, test_vector_reserve_smaller);
    tcase_add_test(tc_lifecycle, test_vector_reserve_larger);
    tcase_add_test(tc_lifecycle, test_vector_free);
    suite_add_tcase(s, tc_lifecycle);

    /* Access test case */
    tc_access = tcase_create("Access");
    tcase_add_test(tc_access, test_vector_get);
    tcase_add_exit_test(tc_access, test_vector_get_crash, 1);
    tcase_add_test(tc_access, test_vector_front_back);
    suite_add_tcase(s, tc_access);

    /* Modifiers test case */
    tc_modifiers = tcase_create("Modifiers");
    tcase_add_test(tc_modifiers, test_vector_set);
    tcase_add_exit_test(tc_modifiers, test_vector_set_crash, 1);
    tcase_add_test(tc_modifiers, test_vector_push_back);
    tcase_add_test(tc_modifiers, test_vector_pop_back);
    tcase_add_exit_test(tc_modifiers, test_vector_pop_back_crash, 1);
    tcase_add_test(tc_modifiers, test_vector_insert);
    tcase_add_test(tc_modifiers, test_vector_erase);
    tcase_add_test(tc_modifiers, test_vector_clear);
    tcase_add_test(tc_modifiers, test_vector_shrink_to_fit);
    suite_add_tcase(s, tc_modifiers);

    /* Status test case */
    tc_status = tcase_create("Status");
    tcase_add_test(tc_status, test_vector_size);
    tcase_add_test(tc_status, test_vector_is_empty);
    suite_add_tcase(s, tc_status);

    return s;
}
