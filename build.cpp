#include "ecow.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto all = unit::create<seq>("all");
  all->add_unit<>("freetype")->add_include_dir("freetype/include");
  all->add_unit<>("harfbuzz");

  return run_main(all, argc, argv);
}
