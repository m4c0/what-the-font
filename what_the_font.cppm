module;
#include "hb-ft.h"

export module what_the_font;

namespace wtf {
export void poc() {
  FT_Library ft_library;
  if (FT_Init_FreeType(&ft_library)) {
    return;
  }

  FT_Face ft_face;
  if (FT_New_Face(ft_library, "dummy.ttf", 0, &ft_face)) {
    return;
  }
}
} // namespace wtf
