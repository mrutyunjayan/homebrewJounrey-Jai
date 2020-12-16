/* date = December 15th 2020 10:08 pm */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>

//~ TYPEDEFS

typedef int8_t         s8;
typedef int16_t        s16;
typedef int32_t        s32;
typedef int64_t        s64;

typedef uint8_t        u8;
typedef uint16_t       u16;
typedef uint32_t       u32;
typedef uint64_t       u64;

typedef int32_t        b32;
typedef unsigned int   uint;

typedef float          f32;
typedef double         f64;
typedef uintptr_t      p64;

#define U32_MAX        0xFFFFFFFF
#define U64_MAX        0xFFFFFFFFFFFFFFFF

//~ MACROS

#define localPersist  static
#define global        static
#define internal      static
#define readOnly      const
#define false         0
#define true          1
#undef  NULL
#define NULL          ((u8*)0)

#define PI            3.14159265359f

#define KB(value)     (value * 1024LL)
#define MB(value)     (KB(value) * 1024LL)
#define GB(value)     (MB(value) * 1024LL)
#define TB(value)     (GB(value) * 1024LL)

#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

#if HOMEBREW_SLOW
#define ASSERT(expression)\
if(!(expression)) {\
*(volatile int*)0 = 0;\
}
#else
#define ASSERT(expression)
#endif

#define STRING(name) #name

#define IS_POWER_OF_2(x) ((x & (x - 1)) == 0)

#define CLAMP(value, min, max) (value <= min) ? min : (value >= max) ? max : value;

#endif //UTILS_H
