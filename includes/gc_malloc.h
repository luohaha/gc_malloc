#ifndef GC_MALLOC_H
#define GC_MALLOC_H

#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

typedef struct block_head_t {
  struct block_head_t *next;
  size_t size; // include block head struct, each's size is PAGE
} block_head_t;

#define PAGE sizeof(block_head_t)

static block_head_t *free_block;
static block_head_t *used_block;

static block_head_t *start_memory;
static block_head_t *end_memory;

#endif
