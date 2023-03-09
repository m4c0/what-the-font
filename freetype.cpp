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
#include "freetype/src/base/ftbitmap.c"
#include "freetype/src/base/ftdebug.c"
#include "freetype/src/base/ftglyph.c"
#include "freetype/src/base/ftinit.c"
#include "freetype/src/base/ftsystem.c"
#include "freetype/src/cff/cff.c"
#include "freetype/src/gzip/ftgzip.c"
#include "freetype/src/psaux/psaux.c"
#include "freetype/src/pshinter/pshinter.c"
#include "freetype/src/psnames/psnames.c"
#include "freetype/src/raster/raster.c"
#include "freetype/src/sfnt/sfnt.c"
#include "freetype/src/smooth/smooth.c"
}
