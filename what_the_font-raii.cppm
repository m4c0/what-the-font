export module what_the_font:raii;
import :error;
import :wrapper;
import hai;

namespace wtf::raii {
struct deleter {
  void operator()(FT_Face f) { FT_Done_Face(f); }
  void operator()(FT_Library f) { FT_Done_FreeType(f); }
  void operator()(hb_font_t *f) { hb_font_destroy(f); }
  void operator()(hb_buffer_t *f) { hb_buffer_destroy(f); }
};

class face {
  hai::value_holder<FT_Face, deleter> m_face{};

public:
  constexpr face() = default;

  face(face &&o) = delete;
  face &operator=(face &&o) = delete;

  face(const face &o) : m_face{*o.m_face} { check(FT_Reference_Face(*m_face)); }
  face &operator=(const face &o) {
    m_face = decltype(m_face){*o.m_face};
    check(FT_Reference_Face(*m_face));
    return *this;
  }

  [[nodiscard]] constexpr auto &operator*() noexcept { return *m_face; }
  [[nodiscard]] constexpr auto &operator*() const noexcept { return *m_face; }
};

class library {
  hai::value_holder<FT_Library, deleter> m_library{};

public:
  library() { check(FT_Init_FreeType(&*m_library)); }

  [[nodiscard]] constexpr auto &operator*() const noexcept {
    return *m_library;
  }
};
} // namespace wtf::raii
