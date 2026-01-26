#include <stdlib.h>
#include <check.h>

/* Import test suites */
Suite *vector_suite(void);

int main(void) {
	int failed;
	SRunner *sr;

	sr = srunner_create(vector_suite());

	srunner_run_all(sr, CK_NORMAL);
	failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
