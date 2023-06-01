module;
#include "hb-ft.h"

export module what_the_font;

namespace wtf {
export class ft_error {
  FT_Error m_error;

public:
  explicit constexpr ft_error(FT_Error err) : m_error{err} {}

  // TODO: use FT_Error_String(err) to get the message
};

void check(FT_Error err) {
  if (err)
    throw ft_error{err};
}

class library {
  FT_Library m_library;

public:
  library() { check(FT_Init_FreeType(&m_library)); }

  [[nodiscard]] auto new_face(const char *font, unsigned size) {
    FT_Face face;
    check(FT_New_Face(m_library, font, 0, &face));

    FT_Set_Char_Size(face, 0, size * 64, 0, 0);
    return face;
  }
};

export void poc(unsigned char *img, unsigned img_w, unsigned img_h) {
  library l{};

  constexpr const auto test_font = "VictorMono-Regular.otf";
  FT_Face ft_face = l.new_face(test_font, 128);

  auto buf = hb_buffer_create();
  hb_buffer_add_utf8(buf, "Coração", -1, 0, -1);
  hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
  hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
  hb_buffer_set_language(buf, hb_language_from_string("pt", -1));

  auto hb_font = hb_ft_font_create_referenced(ft_face);
  hb_shape(hb_font, buf, nullptr, 0);

  unsigned count;
  auto info = hb_buffer_get_glyph_infos(buf, &count);
  auto pos = hb_buffer_get_glyph_positions(buf, &count);
  auto pen_x = 32;
  auto pen_y = 128;
  for (auto i = 0; i < count; i++) {
    check(FT_Load_Glyph(ft_face, info[i].codepoint,
                        FT_LOAD_RENDER | FT_RENDER_MODE_NORMAL));

    auto slot = ft_face->glyph;
    auto &bmp = slot->bitmap;
    for (auto by = 0; by < bmp.rows; by++) {
      for (auto bx = 0; bx < bmp.width; bx++) {
        auto x = bx + slot->bitmap_left + pen_x;
        auto y = by - slot->bitmap_top + pen_y;
        auto bp = by * bmp.pitch + bx;

        if ((x < 0) || (x >= img_w))
          continue;
        if ((y < 0) || (y >= img_h))
          continue;

        auto ip = y * img_w + x;

        auto &ii = img[ip];
        auto bi = bmp.buffer[bp];
        ii = ii > bi ? ii : bi;
      }
    }
    pen_x += pos[i].x_advance / 64;
    pen_y += pos[i].y_advance / 64;
  }

  hb_font_destroy(hb_font);
  hb_buffer_destroy(buf);
}
} // namespace wtf
