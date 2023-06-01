#include "build.hpp"

using namespace ecow;

int main(int argc, char **argv) {
  auto poc = unit::create<exe>("poc");
  poc->add_ref(wtf());
  poc->add_unit<>("poc");
  return run_main(poc, argc, argv);
}
