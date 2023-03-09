extern "C" {
#ifdef __wasm__
struct jmp_buf {
  int x;
};
void abort();
void longjmp(jmp_buf env, int val) { abort(); }
#endif

#define FT_CONFIG_MODULES_H "../../../ftmodule.h"
#define FT2_BUILD_LIBRARY
#include "freetype/src/base/ftbase.c"
#include "freetype/src/base/ftdebug.c"
#include "freetype/src/base/ftinit.c"
#include "freetype/src/base/ftsystem.c"
}
