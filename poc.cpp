import what_the_font;

extern "C" int printf(const char *, ...);

int main() {
  printf("P2\n1024 256 256\n");
  constexpr const auto img_w = 1024;
  constexpr const auto img_h = 256;
  unsigned char img[img_w * img_h]{};

  if (!wtf::poc(img, img_w, img_h))
    return 1;

  for (auto c : img)
    printf("%d ", c);
}
