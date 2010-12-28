#include "g13_util.h"

#define __NO_VERSION__

#ifndef KERNELBUILD 
/* User space */
#warning Building in user space. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define G13_ALLOC(size) malloc(size)
#define G13_PRINT(...) printf(__VA_ARGS__)
#else /* KERNELBUILD */
/* Kernel */
#warning Building for kernel.
#include <linux/slab.h>
#define G13_ALLOC(size) kmalloc(size, GFP_ATOMIC)
#define G13_PRINT(...) printk(__VA_ARGS__)
#endif /* KERNELBUILD */

static void assert_char_array_size(unsigned char* input, int expected_size) {
    int size = sizeof(input) / sizeof(unsigned char);
    if (size != 8) {
        G13_PRINT("wrong size: %d", size);
    }
}

const char* space_between_groups_of_8_chars(const char* input) {
    char* output;
    int input_index = 0;
    int output_index = 0;
    int input_length = strlen(input);
    int output_length = input_length + (input_length / 8) + 1;
    G13_PRINT("input_length: %d\n", input_length);
    output = (char*) G13_ALLOC(output_length);
    for (input_index = 0; input_index < input_length; input_index++) {
        output[output_index] = input[input_index];
        output_index++;
        if (((input_index + 1) % 8) == 0 && input_index < (input_length - 1)) {
            output[output_index] = ' ';
            G13_PRINT("space inserted at %d\n", output_index);
            output_index++;
        }
        G13_PRINT("input index: %d\n", input_index);
    }
    output[output_index] = '\0';
    return output;
}

const char* u64_to_binary_string(const unsigned long long int input) {
    char* output;
    int output_index;
    unsigned long long int remainder = input;
    G13_PRINT("u64_ start\n");
    output = (char*) G13_ALLOC(64 + 1);
    memset(output, '0', 64);
    output[64] = '\0';
    for (output_index = 63; output_index >= 0; output_index--) {
         output[output_index] = remainder % 2 ? '1' : '0';
         remainder = remainder / 2;
         G13_PRINT("remainder = %llu, output = '%s', output_index = %d\n", remainder, output, output_index);
    }
    G13_PRINT("u64_output: %s\n", output);
    return output;
}

const char* eight_bytes_to_bit_string(unsigned char* input) {
    char* output;
    unsigned int output_index;
    unsigned int i;
    unsigned int left_shift;
    unsigned long long int shifting_bitset = 0;
    unsigned long long int result_bitset = 0;
    const char* binary_string;
    const char* grouped_string;
    /* Assert input */ 
    assert_char_array_size(input, 8);
    /* Initialize output */
    output = (char*) G13_ALLOC(64 + 8);
    output_index = 0;
    memset(output, '0', 64 + 8);
    /* Make 64-bit unsigned integer from 8 input chars */
    for (i = 0; i < 8; i++) {
        shifting_bitset = input[i];
        left_shift = 64 - ((i + 1) * 8);
        G13_PRINT("input[%d]: %llu\n", i, shifting_bitset);
        shifting_bitset = shifting_bitset << left_shift;
        G13_PRINT("value (shifted %d): %llu\n", left_shift, shifting_bitset);
        result_bitset = result_bitset | shifting_bitset;
        G13_PRINT("result_bitset at iteration %d: %llu\n", i, result_bitset);
    }
    G13_PRINT("result_bitset final: %llu\n", result_bitset);
    binary_string = u64_to_binary_string(result_bitset);
    G13_PRINT("binary string: %s\n", binary_string);
    grouped_string = space_between_groups_of_8_chars(binary_string);
    G13_PRINT("grouped_string: %s\n", grouped_string);
    return grouped_string;
}


