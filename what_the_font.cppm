export module what_the_font;
export import :error;
import :raii;
import :wrapper;
import hai;
import jute;

namespace wtf {
export struct rect {
  int x;
  int y;
  int w;
  int h;
};

class glyph {
  raii::face m_face;
  hb_glyph_position_t *m_pos;
  hb_glyph_info_t *m_info;

public:
  glyph(raii::face f, hb_glyph_position_t *p, hb_glyph_info_t *i)
      : m_face{f}
      , m_pos{p}
      , m_info{i} {}

  constexpr auto codepoint() const noexcept { return m_info->codepoint; }
  constexpr auto x_advance() const noexcept { return m_pos->x_advance / 64; }
  constexpr auto y_advance() const noexcept { return m_pos->y_advance / 64; }

  constexpr auto bitmap_rect() const noexcept {
    rect res;
    res.x = (*m_face)->glyph->bitmap_left;
    res.y = (*m_face)->glyph->bitmap_top;
    res.w = (*m_face)->glyph->bitmap.width;
    res.h = (*m_face)->glyph->bitmap.rows;
    return res;
  }

  auto load_glyph() {
    check(FT_Load_Glyph(*m_face, m_info->codepoint, ft_load_render));
    return (*m_face)->glyph;
  }

  void blit(unsigned char *img, unsigned img_w, unsigned img_h, unsigned pen_x,
            unsigned pen_y) {
    auto slot = (*m_face)->glyph;
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
  }
};

class glyph_iter {
  raii::face m_face;
  raii::buffer m_buffer;
  unsigned m_idx;

public:
  glyph_iter(raii::face f, raii::buffer b, unsigned idx)
      : m_face{f}
      , m_buffer{b}
      , m_idx{idx} {}

  constexpr auto operator==(const glyph_iter &o) const {
    return m_idx == o.m_idx && *m_buffer == *o.m_buffer;
  }
  constexpr auto &operator++() {
    m_idx++;
    return *this;
  }
  auto operator*() {
    auto info = hb_buffer_get_glyph_infos(*m_buffer, nullptr);
    auto pos = hb_buffer_get_glyph_positions(*m_buffer, nullptr);
    return glyph{m_face, pos + m_idx, info + m_idx};
  }
};
class glyph_list {
  raii::face m_face;
  raii::buffer m_buffer;
  unsigned m_count{};

public:
  explicit glyph_list(raii::face f, raii::buffer b)
      : m_face{f}
      , m_buffer{b}
      , m_count{hb_buffer_get_length(*m_buffer)} {}

  auto begin() { return glyph_iter{m_face, m_buffer, 0}; }
  auto end() { return glyph_iter{m_face, m_buffer, m_count}; }
};

export class buffer {
  raii::face m_face;
  raii::buffer m_buffer;

public:
  buffer(raii::face f, raii::buffer b) : m_face{f}, m_buffer{b} {}

  [[nodiscard]] auto glyphs() const { return glyph_list{m_face, m_buffer}; }

  auto bounding_box() const {
    struct box {
      int w{};
      int h{};
    } res;
    for (auto g : glyphs()) {
      res.w += g.x_advance();
      res.h += g.y_advance();
    }
    return res;
  }

  void draw(unsigned char *img, unsigned img_w, unsigned img_h, int pen_x,
            int pen_y) const {
    draw(img, img_w, img_h, &pen_x, &pen_y);
  }
  void draw(unsigned char *img, unsigned img_w, unsigned img_h, int *pen_x,
            int *pen_y) const {
    for (auto g : glyphs()) {
      g.load_glyph();
      g.blit(img, img_w, img_h, *pen_x, *pen_y);
      *pen_x += g.x_advance();
      *pen_y += g.y_advance();
    }
  }
};

export class face {
  raii::face m_face;
  raii::font m_font;

public:
  explicit face(raii::face f) : m_face{f}, m_font{*f} {}

  // Font size is given in pixels. This dictates the size of the glyph bitmap.
  void set_char_size(unsigned size) {
    FT_Set_Char_Size(*m_face, 0, size * 64, 0, 0);
  }

  [[nodiscard]] auto shape_latin_ltr(jute::view msg, const char *lang) {
    raii::buffer buf{};
    hb_buffer_add_utf8(*buf, msg.data(), msg.size(), 0, -1);
    hb_buffer_set_direction(*buf, HB_DIRECTION_LTR);
    hb_buffer_set_script(*buf, HB_SCRIPT_LATIN);
    hb_buffer_set_language(*buf, hb_language_from_string(lang, -1));
    hb_shape(*m_font, *buf, nullptr, 0);
    return buffer{m_face, buf};
  }

  [[nodiscard]] auto shape_en(jute::view msg) {
    return shape_latin_ltr(msg, "en");
  }
  [[nodiscard]] auto shape_pt(jute::view msg) {
    return shape_latin_ltr(msg, "pt");
  }
};

export class library {
  hai::value_holder<FT_Library, raii::deleter> m_library{};

public:
  library() { check(FT_Init_FreeType(&*m_library)); }

  [[nodiscard]] auto new_face(const char *font) {
    raii::face f{};
    check(FT_New_Face(*m_library, font, 0, &*f));
    return face{f};
  }

  [[nodiscard]] auto new_memory_face(const void *data, unsigned data_size) {
    raii::face f{};
    check(FT_New_Memory_Face(*m_library, static_cast<const FT_Byte *>(data),
                             data_size, 0, &*f));
    return face{f};
  }
};
} // namespace wtf
