#include "g13_util.h"

const u8* format_string_of_64_bits(const u8* input) {
    u8* output;
    u32 input_index = 0;
    u32 output_index = 0;
    u32 input_length = 64;
    u32 output_length = input_length + (input_length / 8) + 1;
    output = G13_ALLOC(output_length * sizeof(u8));
    for (input_index = 0; input_index < input_length; input_index++) {
        output[output_index] = input[input_index];
        output_index++;
        if (((input_index + 1) % 8) == 0 && input_index < (input_length - 1)) {
            output[output_index] = ' ';
            output_index++;
        }
    }
    output[output_index] = '\0';
    return output;
}

const u8* u64_to_binary_string(const u64 input) {
    u8* output;
    u32 output_index;
    u64 remainder = input;
    output = G13_ALLOC((64 + 1) * sizeof(u8));
    memset(output, '0', 64);
    output[64] = '\0';
    /* FIXME VP 28.12.2010: Truly ugly hack to get around unsignedness */
    for (output_index = 63; output_index <= 63; output_index--) {
         u8 bit_value = remainder % 2 ? '1' : '0';
         output[output_index] = bit_value;
         remainder = remainder / 2;
    }
    return output;
}

u64 eight_bytes_to_u64(const u8* input) {
    u64 shifting_bitset = 0;
    u64 result_bitset = 0;
    u32 left_shift; 
    u32 i;
    for (i = 0; i < 8; i++) {
        shifting_bitset = input[i];
        left_shift = 64 - ((i + 1) * 8);
        shifting_bitset = shifting_bitset << left_shift;
        result_bitset = result_bitset | shifting_bitset;
    }
    return result_bitset;
}

const u8* eight_bytes_to_bit_string(const u8* input) {
    return format_string_of_64_bits(u64_to_binary_string(eight_bytes_to_u64(input)));
}

const u8* eight_octets_to_16_nibbles(const u8* input) {
    u32 input_size = 8;
    u32 output_size = input_size * 2;
    u32 i;
    u8 octet;
    u8 high_nibble;
    u8 low_nibble;
    u8* output;
    output = G13_ALLOC(16 * sizeof(u8));
    for (i = 0; i < input_size; i++) {
        octet = input[i];
        high_nibble = octet >> 4;
        low_nibble = octet & 0xf;
        output[i * 2] = high_nibble;
        output[i * 2 + 1] = low_nibble;
    }
    return output;
}


