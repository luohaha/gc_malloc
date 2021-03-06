#include "gc_malloc.h"

void test_malloc_free() {
  gc_debug_print();
  void *a = gc_malloc(16);
  ASSERT(a != NULL, "a != NULL");
  gc_debug_print();
  void *b = gc_malloc(32);
  ASSERT(b != NULL, "b != NULL");
  gc_debug_print();
  void *c = gc_malloc(64);
  ASSERT(c != NULL, "c != NULL");
  gc_debug_print();
  gc_free(c);
  gc_debug_print();
  gc_free(b);
  gc_debug_print();
  gc_free(a);
  gc_debug_print();
}

int main() {
  ASSERT(1 == 1, "1 == 1");
  gc_init(512);
  test_malloc_free();
  return 0;
}
