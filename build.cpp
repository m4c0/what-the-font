#include "ecow.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto m = unit::create<mod>("what_the_font");
  m->add_include_dir("harfbuzz/src");
  m->add_include_dir("freetype/include");
  m->add_unit<>("freetype")->add_include_dir("freetype/include");
  m->add_unit<>("harfbuzz");

  auto poc = unit::create<app>("poc");
  poc->add_ref(m);
  poc->add_unit<>("poc");
  return run_main(poc, argc, argv);
}
