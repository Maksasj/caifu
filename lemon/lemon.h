#ifndef LEMON_H
#define LEMON_H

typedef unsigned char lemon_u8_t;
typedef unsigned short lemon_u16_t;
typedef unsigned int lemon_u32_t;
typedef unsigned long long lemon_u64_t;

typedef signed char lemon_i8_t;
typedef signed short lemon_i16_t;
typedef signed int lemon_i32_t;
typedef signed long long lemon_i64_t;

#ifndef LEMON_SIZE_T
    #define LEMON_SIZE_T lemon_u64_t
#endif

typedef LEMON_SIZE_T lemon_size_t;
typedef lemon_u8_t lemon_byte_t;

// Fingerprint is used for memory corruption
#define LEMON_FINGERPRINT 0x824d268524c2d0c2 
#define LEMON_NULL ((void*) 0)

#ifndef LEMON_INLINE
    #define LEMON_INLINE static inline
#endif

#ifndef LEMON_ASSERT
    #include <assert.h>
    #define LEMON_ASSERT assert
#endif

#ifndef LEMONA_PRINT
    #include <stdio.h>
    #define LEMONA_PRINT printf
#endif

typedef struct lemon_chunk_t {
    lemon_size_t fingerprint;

	lemon_size_t size;
	lemon_byte_t inuse;
    
	struct lemon_chunk_t* next;
	struct lemon_chunk_t* prev;
} lemon_chunk_t;

extern lemon_byte_t* lemon_memp; 

LEMON_INLINE void lemon_init_u64(lemon_byte_t* heap, lemon_u64_t size);
LEMON_INLINE void lemon_init_i64(lemon_byte_t* heap, lemon_i64_t size);
LEMON_INLINE void lemon_init_u32(lemon_byte_t* heap, lemon_u32_t size);
LEMON_INLINE void lemon_init_i32(lemon_byte_t* heap, lemon_i32_t size);
LEMON_INLINE void lemon_init(lemon_byte_t* heap, lemon_size_t size);

LEMON_INLINE void* lemon_malloc_u64(lemon_u64_t size);
LEMON_INLINE void* lemon_malloc_i64(lemon_i64_t size);
LEMON_INLINE void* lemon_malloc_u32(lemon_u32_t size);
LEMON_INLINE void* lemon_malloc_i32(lemon_i32_t size);
LEMON_INLINE void* lemon_malloc(lemon_size_t size);

LEMON_INLINE void lemon_free(void* block);

LEMON_INLINE void lemon_print();

#ifdef LEMON_IMPLEMENTATION

lemon_byte_t* lemon_memp = LEMON_NULL;

LEMON_INLINE void lemon_init_u64(lemon_byte_t* heap, lemon_u64_t size) {
    lemon_memp = heap;

    lemon_chunk_t* first = (lemon_chunk_t*) lemon_memp;

    first->size = size - sizeof(lemon_chunk_t);
    first->inuse = 0;
    first->next = LEMON_NULL;
    first->prev = LEMON_NULL;
    first->fingerprint = LEMON_FINGERPRINT;
}

LEMON_INLINE void lemon_init_i64(lemon_byte_t* heap, lemon_i64_t size) {
    lemon_init_u64(heap, (lemon_u64_t) size);
}

LEMON_INLINE void lemon_init_u32(lemon_byte_t* heap, lemon_u32_t size) {
    lemon_init_u64(heap, (lemon_u64_t) size);
}

LEMON_INLINE void lemon_init_i32(lemon_byte_t* heap, lemon_i32_t size) {
    lemon_init_u64(heap, (lemon_u64_t) size);
}

LEMON_INLINE void lemon_init(lemon_byte_t* heap, lemon_size_t size) {
    lemon_init_u64(heap, (lemon_u64_t) size);
}

LEMON_INLINE void* lemon_malloc_u64(lemon_u64_t size) {
    lemon_chunk_t* chunk = (lemon_chunk_t*) lemon_memp; 
    
    LEMON_ASSERT(chunk->fingerprint == LEMON_FINGERPRINT);

    while((chunk->inuse == 1) || (size > chunk->size)) {
        chunk = chunk->next;

        if(chunk == LEMON_NULL)
            return LEMON_NULL;
    }

    LEMON_ASSERT(chunk->fingerprint == LEMON_FINGERPRINT);

    void* ret = (void*) ((lemon_byte_t*) chunk) + sizeof(lemon_chunk_t);
    lemon_chunk_t* next = (lemon_chunk_t*) (((lemon_byte_t*) ret) + size);

    next->size = chunk->size - sizeof(lemon_chunk_t) - size;
    next->next = LEMON_NULL;
    next->inuse = 0;
    next->prev = chunk;
    next->fingerprint = LEMON_FINGERPRINT;

    chunk->size = size;
    chunk->inuse = 1;
    chunk->next = next;

    return ret;  
}

LEMON_INLINE void* lemon_malloc_i64(lemon_i64_t size) {
    return lemon_malloc_u64((lemon_u64_t) size);
}

LEMON_INLINE void* lemon_malloc_u32(lemon_u32_t size) {
    return lemon_malloc_u64((lemon_u64_t) size);
}

LEMON_INLINE void* lemon_malloc_i32(lemon_i32_t size) {
    return lemon_malloc_u64((lemon_u64_t) size);
}

LEMON_INLINE void* lemon_malloc(lemon_size_t size) {
    return lemon_malloc_u64(size);
}

LEMON_INLINE void lemon_free(void* ptr) {
    if(ptr == LEMON_NULL)
        return;
    
    lemon_chunk_t* chunk = (lemon_chunk_t*) (((lemon_byte_t*) ptr) - sizeof(lemon_chunk_t));
    LEMON_ASSERT(chunk->fingerprint == LEMON_FINGERPRINT);

    lemon_chunk_t* prev = chunk->prev;
    lemon_chunk_t* next = chunk->next;

    chunk->inuse = 0;

    if(prev != LEMON_NULL && prev->inuse == 0) {
        LEMON_ASSERT(prev->fingerprint == LEMON_FINGERPRINT);

        prev->size += sizeof(lemon_chunk_t) + chunk->size;
        prev->next = next;
        chunk = prev;
    }

    if(next != LEMON_NULL && next->inuse == 0) {
        LEMON_ASSERT(next->fingerprint == LEMON_FINGERPRINT);

        chunk->size += sizeof(lemon_chunk_t) + next->size;
        chunk->next = next->next;
    }
}

LEMON_INLINE void lemon_print() {
    lemon_chunk_t* chunk = (lemon_chunk_t*) lemon_memp; 

    LEMON_ASSERT(chunk->fingerprint == LEMON_FINGERPRINT);

    while(1) {
        LEMONA_PRINT("[%llu; %d],", chunk->size, chunk->inuse);

        if(chunk->next == LEMON_NULL)
            break;

        chunk = chunk->next;
    }

    LEMONA_PRINT("\n");
}

#endif

#endif 