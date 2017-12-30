#ifndef GC_MALLOC_H
#define GC_MALLOC_H

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

#define ASSERT(exp, id)				\
  if (!(exp)) {					\
    fprintf(stderr, "TEST %s FAIL\n", id); \
    exit(-1);   \
  } else {     \
    fprintf(stderr, "TEST %s SUCCESS\n", id); \
  }

typedef struct block_head_t {
  struct block_head_t *next;
  size_t size; // include block head struct, each's size is PAGE
} block_head_t;

#define PAGE sizeof(block_head_t)
#define MARK(ptr) (ptr->next = ((size_t) (ptr->next)) | 0x1)
#define UNMARK(ptr) (ptr->next = ((size_t) (ptr->next)) & (~0x1))
#define PEEK_UNMARK(ptr) (((size_t) (ptr) & (~0x1))
#define IS_MARK(ptr) ((((size_t) (ptr->next)) & 0x1) == 0x1)

// gc_log.c
extern inline void gc_debug(const char* msg);
extern inline void gc_warn(const char* msg);
extern inline void gc_error(const char* msg);

// gc_malloc.c
extern void* gc_malloc(size_t byte_size);
extern void gc_free(void *ptr);
extern void gc_init(size_t size_byte);
extern void gc_debug_print();
extern block_head_t* find_block_in_used(size_t *ptr);

#endif
