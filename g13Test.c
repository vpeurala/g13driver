#include "CuTest.h"
#include "g13_util.h"

void Test64BitDisplay(CuTest* tc) {
    unsigned const char input[] = {1, 2, 3, 4, 5, 6, 7, 8};
    unsigned const char* output = eight_bytes_to_bit_string(input);
    CuAssertPtrNotNull(tc, output);
    CuAssertStrEquals(tc, (char*) "00000001 00000010 00000011 00000100 00000101 00000110 00000111 00001000", (char*) output);
}

void Test8OctetsTo16Nibbles(CuTest* tc) {
    unsigned const char input[] = {1, 2, 3, 4, 255, 126, 60, 24};
    unsigned const char* output = eight_octets_to_16_nibbles(input);
    CuAssertPtrNotNull(tc, output);
    
    CuAssertIntEquals(tc, 0, output[0]);
    CuAssertIntEquals(tc, 1, output[1]);
    CuAssertIntEquals(tc, 0, output[2]);
    CuAssertIntEquals(tc, 2, output[3]);
    CuAssertIntEquals(tc, 0, output[4]);
    CuAssertIntEquals(tc, 3, output[5]);
    CuAssertIntEquals(tc, 0, output[6]);
    CuAssertIntEquals(tc, 4, output[7]);
    CuAssertIntEquals(tc, 15, output[8]);
    CuAssertIntEquals(tc, 15, output[9]);
    CuAssertIntEquals(tc, 7, output[10]);
    CuAssertIntEquals(tc, 14, output[11]);
    CuAssertIntEquals(tc, 3, output[12]);
    CuAssertIntEquals(tc, 12, output[13]);
    CuAssertIntEquals(tc, 1, output[14]);
    CuAssertIntEquals(tc, 8, output[15]);
}

CuSuite* G13Suite(void) {
    printf("G13Suite");
    CuSuite* suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, Test64BitDisplay);
    SUITE_ADD_TEST(suite, Test8OctetsTo16Nibbles);
    return suite;
}

