#include "gc_malloc.h"


inline void gc_debug(const char* msg) {
#ifdef _GC_LOG_DEBUG
  fprintf(stderr, "[debug] : %s\n", msg);
#endif
}

inline void gc_warn(const char* msg)≈ {
#ifdef _GC_LOG_WARN
  fprintf(stderr, "[warn] : %s\n", msg);
#endif
}

inline void gc_error(const char* msg) {
  fprintf(stderr, "[error] : %s\n", msg);
  exit(-1);
}
