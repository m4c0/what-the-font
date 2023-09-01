import jute;
import what_the_font;

extern "C" int printf(const char *, ...);

int main() {
  wtf::library l{};

  constexpr const auto img_w = 1024;
  constexpr const auto img_h = 256;
  unsigned char img[img_w * img_h]{};

  constexpr const auto lines = 4;
  constexpr const auto margin_y = 13;
  constexpr const auto font_h = (img_h - 2 * margin_y) / lines;

  constexpr const auto margin_x = margin_y;

  constexpr const auto test_font = "VictorMono-Regular.otf";
  auto f = l.new_face(test_font, font_h);

  constexpr jute::view text[]{
      "Minha terra tem palmeiras,",
      "Onde canta o Sabiá;",
      "As aves, que aqui gorjeiam,",
      "Não gorjeiam como lá.",
  };
  unsigned lnum = 1;
  for (auto line : text) {
    auto x = margin_x;
    auto y = margin_y + font_h * lnum++;
    f.shape_pt(line).draw(img, img_w, img_h, x, y);
  }

  printf("P2\n1024 256 256\n");
  for (auto c : img)
    printf("%d ", c);
}
