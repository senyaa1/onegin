#include <stdint.h>

#define PTR_PACK(ptr, length) (void*)(((uint64_t)length << (8 * 6)) | (uint64_t)ptr)
#define PTR_STRIP(ptr) (void*)((uint64_t)ptr & (~(0xFFFFull << (6 * 8))))
#define PTR_GETLEN(ptr) (uint16_t)(((0xFFFFull << (8 * 6)) & (uint64_t)ptr) >> (8 * 6))
