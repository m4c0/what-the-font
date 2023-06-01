#include "ecow.hpp"

auto wtf() {
  using namespace ecow;

  auto m = unit::create<mod>("what_the_font");
  m->add_include_dir("harfbuzz/src");
  m->add_include_dir("freetype/include");
  m->add_unit<>("freetype")->add_include_dir("freetype/include");
  m->add_unit<>("harfbuzz")->add_include_dir("freetype/include");
  return m;
}
