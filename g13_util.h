#ifndef G13_UTIL_H
#define G13_UTIL_H

#define __NO_VERSION__

#ifndef KERNELBUILD 
/* User space */
#warning Building for user space.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define G13_ALLOC(size) malloc(size)
#define G13_ASSERT(condition) assert(condition)
#define G13_PRINT(...) printf(__VA_ARGS__)
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8  unsigned char
#else /* KERNELBUILD */
/* Kernel */
#warning Building for kernel.
#include <linux/slab.h>
#define G13_ALLOC(size) kmalloc(size, GFP_ATOMIC)
#define G13_ASSERT(condition) if (!condition) printk(KERN_CRIT "G13: Assert failed!\n") 
#define G13_PRINT(...) printk(__VA_ARGS__)
#endif /* KERNELBUILD */

typedef struct {
    u8 joystick_x;
    u8 joystick_y;
    u8 joystick_buttons;
    u32 keys;
    u16 specials;
} G13State;

const u8* format_string_of_64_bits(const u8* input);

const u8* u64_to_binary_string(const u64 input);

u64 eight_bytes_to_u64(const u8* input);

const u8* eight_bytes_to_bit_string(const u8* input);

const u8* eight_octets_to_16_nibbles(const u8* input);

#endif /* G13_UTIL_H */

