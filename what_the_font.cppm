module;
#include "hb-ft.h"

export module what_the_font;

namespace wtf {
constexpr const auto test_font = "VictorMono-Regular.otf";

export [[nodiscard]] int poc() {
  FT_Library ft_library;
  if (auto err = FT_Init_FreeType(&ft_library)) {
    return err;
  }

  FT_Face ft_face;
  if (auto err = FT_New_Face(ft_library, test_font, 0, &ft_face)) {
    return err;
  }

  FT_Set_Char_Size(ft_face, 0, 1000, 0, 0);

  auto hb_buf = hb_buffer_create();
  hb_buffer_set_direction(hb_buf, HB_DIRECTION_LTR);
  hb_buffer_set_script(hb_buf, HB_SCRIPT_LATIN);
  hb_buffer_set_language(hb_buf, hb_language_from_string("pt", -1));

  auto hb_font = hb_ft_font_create_referenced(ft_face);

  hb_font_destroy(hb_font);
  hb_buffer_destroy(hb_buf);
  return 0;
}
} // namespace wtf
