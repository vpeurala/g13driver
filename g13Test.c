#include <stdio.h>
#include "CuTest.h"
#include "g13_util.h"

void Test64BitDisplay(CuTest* tc) {
    unsigned char input[] = {1, 2, 3, 4, 5, 6, 7, 8};
    const char* output = eight_bytes_to_bit_string(input);
    CuAssertPtrNotNull(tc, output);
    CuAssertStrEquals(tc, "00000001 00000010 00000011 00000100 00000101 00000110 00000111 00001000", output);
    printf("output: '%s'", output);
}

CuSuite* G13Suite(void) {
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Test64BitDisplay);
    return suite;
}

