module;
#pragma leco add_impl freetype harfbuzz
#pragma leco add_include_dir "freetype/include"
#include "harfbuzz/src/hb-ft.h"

export module what_the_font;
import hai;
import jute;

namespace wtf {
export class ft_error {
  FT_Error m_error;

public:
  explicit constexpr ft_error(FT_Error err) : m_error{err} {}

  // TODO: use FT_Error_String(err) to get the message
};
void check(FT_Error err) {
  if (!err)
    return;

#ifdef __wasm__
  abort();
#else
  throw ft_error{err};
#endif
}

struct deleter {
  void operator()(FT_Face f) { FT_Done_Face(f); }
  void operator()(hb_font_t *f) { hb_font_destroy(f); }
  void operator()(hb_buffer_t *f) { hb_buffer_destroy(f); }
};

class glyph_iter {
  unsigned m_idx;
  hb_glyph_position_t *m_pos{};
  hb_glyph_info_t *m_info{};

public:
  glyph_iter(hb_glyph_position_t *p, hb_glyph_info_t *i, unsigned idx)
      : m_idx{idx}
      , m_pos{p}
      , m_info{i} {}

  constexpr auto operator==(const glyph_iter &o) const {
    return m_idx == o.m_idx && m_pos == o.m_pos && m_info == o.m_info;
  }
  constexpr auto &operator++() {
    m_idx++;
    return *this;
  }
  constexpr auto operator*() { return m_idx; }
};
class glyph_list {
  unsigned m_count{};
  hb_glyph_position_t *m_pos{};
  hb_glyph_info_t *m_info{};

public:
  explicit glyph_list(hb_buffer_t *b) {
    m_info = hb_buffer_get_glyph_infos(b, &m_count);
    m_pos = hb_buffer_get_glyph_positions(b, &m_count);
  }

  auto begin() const { return glyph_iter{m_pos, m_info, 0}; }
  auto end() const { return glyph_iter{m_pos, m_info, m_count}; }
};

class buffer {
  hai::value_holder<FT_Face, deleter> m_face;
  hai::value_holder<hb_buffer_t *, deleter> m_buffer;

  [[nodiscard]] auto load_glyph(unsigned index) const {
    constexpr const auto flags = FT_LOAD_RENDER | FT_RENDER_MODE_NORMAL;
    check(FT_Load_Glyph(*m_face, index, flags));

    return (*m_face)->glyph;
  }

public:
  buffer(FT_Face f, hb_buffer_t *b) : m_face{f}, m_buffer{b} {
    FT_Reference_Face(f);
  }

  [[nodiscard]] auto glyphs() { return glyph_list{*m_buffer}; }

  auto bounding_box() const {
    struct box {
      int w{};
      int h{};
    } res;
    unsigned count;
    auto pos = hb_buffer_get_glyph_positions(*m_buffer, &count);
    for (auto i = 0; i < count; i++) {
      res.w += pos[i].x_advance / 64;
      res.h += pos[i].y_advance / 64;
    }
    return res;
  }

  void draw(unsigned char *img, unsigned img_w, unsigned img_h, int pen_x,
            int pen_y) const {
    draw(img, img_w, img_h, &pen_x, &pen_y);
  }
  void draw(unsigned char *img, unsigned img_w, unsigned img_h, int *pen_x,
            int *pen_y) const {
    unsigned count;
    auto info = hb_buffer_get_glyph_infos(*m_buffer, &count);
    auto pos = hb_buffer_get_glyph_positions(*m_buffer, &count);
    for (auto i = 0; i < count; i++) {
      auto slot = load_glyph(info[i].codepoint);
      auto &bmp = slot->bitmap;
      for (auto by = 0; by < bmp.rows; by++) {
        for (auto bx = 0; bx < bmp.width; bx++) {
          auto x = bx + slot->bitmap_left + *pen_x;
          auto y = by - slot->bitmap_top + *pen_y;
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
      *pen_x += pos[i].x_advance / 64;
      *pen_y += pos[i].y_advance / 64;
    }
  }
};

export class face {
  hai::value_holder<FT_Face, deleter> m_face;
  hai::value_holder<hb_font_t *, deleter> m_font;

public:
  explicit face(FT_Face f)
      : m_face{f}, m_font{hb_ft_font_create_referenced(f)} {}

  [[nodiscard]] constexpr auto operator*() const noexcept { return *m_font; }

  [[nodiscard]] auto shape_latin_ltr(jute::view msg, const char *lang) {
    auto buf = hb_buffer_create();
    hb_buffer_add_utf8(buf, msg.data(), msg.size(), 0, -1);
    hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
    hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language(buf, hb_language_from_string(lang, -1));
    hb_shape(*m_font, buf, nullptr, 0);
    return buffer{*m_face, buf};
  }

  [[nodiscard]] auto shape_en(jute::view msg) {
    return shape_latin_ltr(msg, "en");
  }
  [[nodiscard]] auto shape_pt(jute::view msg) {
    return shape_latin_ltr(msg, "pt");
  }
};

export class library {
  FT_Library m_library;

public:
  library() { check(FT_Init_FreeType(&m_library)); }

  [[nodiscard]] auto new_face(const char *font, unsigned size) {
    FT_Face f;
    check(FT_New_Face(m_library, font, 0, &f));

    FT_Set_Char_Size(f, 0, size * 64, 0, 0);
    return face{f};
  }

  [[nodiscard]] auto new_memory_face(const void *data, unsigned data_size,
                                     unsigned size) {
    FT_Face f;
    check(FT_New_Memory_Face(m_library, static_cast<const FT_Byte *>(data),
                             data_size, 0, &f));

    FT_Set_Char_Size(f, 0, size * 64, 0, 0);
    return face{f};
  }
};
} // namespace wtf
