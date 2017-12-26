#include "gc_malloc.h"

void gc_debug(const char* msg);
void gc_warn(const char* msg);
void gc_error(const char* msg);

/**
   创建block头部
 **/
static block_head_t *build_block(block_head_t *addr, size_t size, block_head_t *next) {
  if (addr == NULL || next == NULL) {
    gc_error("init_meory: addr or next is NULL");
  }
  if (size <= 1) {
    gc_error("build_block : size is less than 1");∑
  }
  block_head_t *block = (block_head_t*) addr;
  block->next = next;
  block->size = size;
  return block;
}

/**
   初始化内存池
**/
static void init_memory_pool(size_t size) {
  if (size <= 1) {
    gc_error("init_meory: size is less than 1");
  }
  start_memory = sbrk(size * PAGE);
  if (start_memory == -1) {
    gc_error("init_memory : sbrk return -1");
  }
  start_memory += size;
  free_block = build_block(start_memory, size, start_memory);
}

/**
   根据size，分配内存块
**/
static block_head_t *malloc_block(size_t size) {
  if (size <= 1) {
    gc_error("malloc_block: size is less than 1");
  }
  block_head_t *find_ptr = free_block->next;
  block_head_t *prev_ptr = free_block;
  block_head_t *start_ptr = find_ptr;
  if (find_ptr == NULL) {
    gc_warn("malloc_block: memory pool is empty");
    return NULL;
  }
  do {
    if (find_ptr->size >= size) {
      // find it
      if (find_ptr->size - size > 1) {
	block_head_t *new_ptr = build_block(find_ptr + size, find_ptr->size - size, find_ptr->next);
	prev_ptr->next = new_ptr;
	find_ptr->next = NULL;
	find_ptr->size = size;
	free_block = prev_ptr;
	return find_ptr;
      } else {
	if (find_ptr->next == find_ptr) {
	  // last one block
	  free_block = NULL;
	  find_ptr->next = NULL;
	  return find_ptr;
	} else {
	  // more than one block
	  free_block = prev_ptr;
	  prev_ptr->next = find_ptr->next;
	  find_ptr->next = NULL;
	  return find_ptr;
	}
      }
    } else {
      prev_ptr = find_ptr;
      find_ptr = find_ptr->next;
    }
  } while (prev_ptr != start_ptr);
  // can not find useful block
  gc_warn("malloc_block: can not find useful block");
  return NULL;
}

/**
   right > left && left < mid && mid < right
**/
static void insert_block_in_mid(block_head_t *left, block_head_t *mid, block_head_t *right) {
  mid->next = right;
  left->next = mid;
  if (mid + mid->size == right) {
    mid->size += right->size;
    mid->next = right->next;
  }
  if (left + left->size == mid) {
    left->size += mid->size;
    left->next = mid->next;
  }
}

/**
   left >= right && mid > left
**/
static void insert_block_in_right(block_head_t *left, block_head_t *mid, block_head_t *right) {
  mid->next = right;
  left->next = mid;
  if (left + left->size == mid) {
    left->size += mid->size;
    left->next = mid->next;
  }
}

/**
   left >= right && mid < right
**/
static void insert_block_in_left(block_head_t *left, block_head_t *mid, block_head_t *right) {
  mid->next = right;
  left->next = mid;
  if (mid + mid>size == right) {
    mid->size += right->size;
    mid->next = right->next;
  }
}

/**
   回收已用内存块
**/
static void free_block(block_head_t *block) {
  if (block == NULL) {
    gc_error("free_block : block is NULL");
  }
  if (block > end_memory || block < start_memory) {
    gc_error("free_block : block is out of memory pool");
  }
  if (free_block == NULL) {
    free_block = block;
    return;
  }
  block_head_t *find_ptr = free_block;
  do {
    if (find_ptr < block && block < find_ptr->next) {
      // insert and merge
      insert_block_in_mid(find_ptr, block, find_ptr->next);
    } else if (find_ptr >= find_ptr->next) {
      if (block > find_ptr) {
	insert_block_in_right(find_ptr, block, find_ptr->next);
      } else if (block < find_ptr->next) {
	insert_block_in_left(find_ptr, block, find_ptr->next);
      } else {
	gc_error("free_block : can't happen");
      }
    } else {
      find_ptr = find_ptr->next;
    }
  } while (find_ptr != free_block);
  gc_error("free_block : can't find a place for this block");
}

static void add_to_used_block(block_head_t *block) {
  if (block == NULL) {
    gc_error("add_to_used_block : block is NULL");
  }
  if (block < start_memory || block > end_memory) {
    gc_error("add_to_used_block : block is out of memory pool");
  }
  if (used_block == NULL) {
    used_block = block;
  } else {
    block->next = used_block->next;
    used_block->next = block;
  }
}

static int is_block_in_used(block_head_t *block) {
  block_head_t *find_ptr = used_block;
  do {
    if (find_ptr == block) {
      return 1;
    } else {
      find_ptr = find_ptr->next;
    }
  } while (find_ptr != used_block);
  return 0;
}

static block_head_t *remove_block_from_used(block_head_t *block) {
  block_head_t *find_ptr = used_block->next;
  block_head_t *prev_ptr = used_block;
  do {
    if (find_ptr == block) {
      prev_ptr->next = find_ptr->next;
      return find_ptr;
    } else {
      prev_ptr = find_ptr;
      find_ptr = find_ptr->next;
    }
  } while (prev_ptr != used_block);
  return NULL;
}

void *gc_malloc(size_t size_byte) {
  if (size_byte == 0)
    return NULL;
  int size = 0;
  if ((size_byte % PAGE) != 0) {
    size = size_byte / PAGE + 1;
  } else {
    size = size_byte / PAGE;
  }
  size++; // add head struct's size
  block_head_t *block = malloc_block(size);
  if (block == NULL) {
    gc_warn("gc_malloc : malloc fail");
    return NULL;
  }
  add_to_used_block(block);
  return block + 1;
}

void gc_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  if (ptr < start_memory || ptr > end_memory) {
    return;
  }
  block_head_t *block = (block_head_t*) ((void *)ptr - PAGE);
  if (remove_block_from_used(block) == NULL) {
    return;
  }
  free_block(block);
}
