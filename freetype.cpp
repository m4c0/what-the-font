extern "C" {
#ifdef __wasm__
struct jmp_buf {
  int x;
};
void abort();
void longjmp(jmp_buf env, int val) { abort(); }
int setjmp(jmp_buf env) {}
#endif

#define FT_CONFIG_MODULES_H "ftmodule.h"
#define FT_CONFIG_OPTION_ERROR_STRINGS
#define FT2_BUILD_LIBRARY
#include "ext/freetype/src/base/ftbase.c"
#include "ext/freetype/src/base/ftbitmap.c"
#include "ext/freetype/src/base/ftdebug.c"
#include "ext/freetype/src/base/ftglyph.c"
#include "ext/freetype/src/base/ftinit.c"
#include "ext/freetype/src/base/ftsystem.c"
#include "ext/freetype/src/cff/cff.c"
#include "ext/freetype/src/gzip/ftgzip.c"
#include "ext/freetype/src/psaux/psaux.c"
#include "ext/freetype/src/pshinter/pshinter.c"
#include "ext/freetype/src/psnames/psnames.c"
#include "ext/freetype/src/raster/raster.c"
#include "ext/freetype/src/sfnt/sfnt.c"
#include "ext/freetype/src/smooth/smooth.c"
#include "ext/freetype/src/truetype/truetype.c"
}
