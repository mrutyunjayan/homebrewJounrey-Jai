/* date = December 23rd 2020 0:55 pm */

#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <string.h>

//~MEMORY ARENA

// https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/ for reference

typedef struct Arena {
    u8* base;
    size_t size;
    u64 offset;
} Arena;

internal Arena
arena_initialize(size_t size, 
                 u8* base) {
    
    Arena result = {
        .base = base,
        .size = size,
        .offset = 0
    };
    
    return result;
}

internal void
arena_free(Arena* arena) {
    arena->offset = 0;
    memset(arena->base, 0, arena->size);
}

internal p64
arena_align_forward(p64 ptr, 
                    size_t align) {
    
    p64 p, a, modulo;
    
    ASSERT(IS_POWER_OF_2(align));
    
    p = ptr;
    a = (p64)align;
    
    modulo = p & (a - 1);
    
    if (modulo != 0) {
        // If p is not aligned, push the address to the
        // next aligned value
        p += a - modulo;
    }
    
    return p;
}

internal void* 
arena_alloc_align(Arena* arena, 
                  u64 size,
                  size_t align) {
    
    p64 currentPtr = (p64)arena->base + (p64)arena->offset;
    p64 offsetAlignedForward = arena_align_forward(currentPtr, align);
    offsetAlignedForward -= (p64)arena->base; // Change to relative offset
    
    //Check to see if the backing memory has space left
    if (offsetAlignedForward + size <= arena->size) {
        
        void* ptr = &arena->base[offsetAlignedForward];
        arena->offset = offsetAlignedForward + size;
        
        return ptr;
    }
    
    // return NULL if the arena is out of memory
    return NULL;
}

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif

// Because C doesn't have default parameters
internal void*
arena_alloc(Arena* arena,
            size_t size) {
    return arena_alloc_align(arena, size, DEFAULT_ALIGNMENT);
}

#define ARENA_PUSH_STRUCT(arena, type) (type*)arena_alloc(arena, sizeof(type)) 
#define ARENA_PUSH_ARRAY(arena, count, type) (type*)arena_alloc(arena, (count * sizeof(type)))

#endif //CONTAINERS_H
