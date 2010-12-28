#include "CuTest.h"
#include "g13_util.h"

void Test64BitDisplay(CuTest* tc) {
    printf("start_test");
    unsigned const char input[] = {1, 2, 3, 4, 5, 6, 7, 8};
    unsigned const char* output = eight_bytes_to_bit_string(input);
    printf("run");
    CuAssertPtrNotNull(tc, output);
    CuAssertStrEquals(tc, (char*) "00000001 00000010 00000011 00000100 00000101 00000110 00000111 00001000", (char*) output);
    printf("output: '%s'", output);
}

CuSuite* G13Suite(void) {
    printf("G13Suite");
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Test64BitDisplay);
    return suite;
}

