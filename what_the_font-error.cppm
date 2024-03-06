export module what_the_font:error;
import :wrapper;

namespace wtf {
export class ft_error {
  FT_Error m_error;

public:
  explicit constexpr ft_error(FT_Error err) : m_error{err} {}

  auto what() const { return FT_Error_String(m_error); }
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
} // namespace wtf
