#include <stdio.h>
#include <assert.h>
#include "vector.h"

const int TEST_DEFAULT_VECTOR_CAPACITY = 20;

void testVectorInitialisation(void) {
	Vector v;

	vector_init(&v, sizeof(int));

	assert(v.size == 0);
	assert(v.element_size == sizeof(int));
	assert(v.capacity == TEST_DEFAULT_VECTOR_CAPACITY);
	assert(v.items != NULL);

	fprintf(stdout, "PASSED: testVectorInitialisation\n");
}

void testVectorReserve(void) {
	Vector v;

	vector_init(&v, sizeof(int));
	
	{
		vector_reserve(&v, 5);
		assert(v.capacity == TEST_DEFAULT_VECTOR_CAPACITY);
		assert(v.items != NULL);
	}

	{
		vector_reserve(&v, 50);
		assert(v.capacity == 50);
		assert(v.items != NULL);
	}

	fprintf(stdout, "PASSED: testVectorReserve\n");
}

void testVectorFree(void) {
	Vector v;

	vector_init(&v, sizeof(int));
	vector_free(&v);

	assert(v.capacity == 0);
	assert(v.size == 0);
	assert(v.items == NULL);

	fprintf(stdout, "PASSED: testVectorFree\n");
}

int main(void) {
	testVectorInitialisation();
	testVectorReserve();
	testVectorFree();

	fprintf(stdout, "All tests passed");

	return 0;
}
