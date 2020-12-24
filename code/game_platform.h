/* date = December 23rd 2020 8:01 pm */

#ifndef GAME_PLATFORM_H
#define GAME_PLATFORM_H

typedef struct game_Memory {
    void* transientStorage;
    void* persistentStorage;
    
    u64 transientStorageSize;
    u64 persistentStorageSize;
    
    b32 isInitialized;
} game_Memory;


#endif //GAME_PLATFORM_H
