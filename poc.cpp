import what_the_font;

extern "C" int printf(const char *, ...);

int main() {
  wtf::library l{};

  constexpr const auto test_font = "VictorMono-Regular.otf";
  auto f = l.new_face(test_font, 128);

  constexpr const auto img_w = 1024;
  constexpr const auto img_h = 256;
  unsigned char img[img_w * img_h]{};

  f.shape_pt("> Coração").draw(img, img_w, img_h, 32, 128);
  f.shape_pt("> Estudante").draw(img, img_w, img_h, 32, 256);

  printf("P2\n1024 256 256\n");
  for (auto c : img)
    printf("%d ", c);
}
