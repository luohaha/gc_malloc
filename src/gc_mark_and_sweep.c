#include "gc_malloc.h"

static void mark(size_t* start_ptr, size_t* end_ptr) {
  size_t* p = start_ptr;
  for (; p < end_ptr; p++) {
    block_head_t *find = find_block_in_used(p);
    if (find != NULL) {
      MARK(find);
    }
  }
}

static void mark_block_chain(block_head_t* ptr) {
  if (!IS_MARK(ptr))
    return;
  do {
    ptr = PEEK_UNMARK(ptr->next);
    if (IS_MARK(ptr)) {
      break;
    } else {
      MARK(ptr);
    }
  } while(true);
}

static void mark_heap() {
  block_head_t *ptr = used_block;
  do {
    mark_block_chain(ptr);
    ptr = PEEK_UNMARK(ptr->next);
  } while (ptr != used_block);
}

static void mark_bbs() {

}

static void mark_stack() {

}

static void sweep() {
  block_head_t *ptr = used_block;
  do {
    if (IS_MARK(ptr)) {

    } else {
      
      remove_block_from_used(ptr);
    }
    ptr = PEEK_UNMARK(ptr->next);
  } while (ptr != used_block);
}

static void gc() {
  mark_bbs();
  mark_stack();
  mark_heap();
  sweep();
}
