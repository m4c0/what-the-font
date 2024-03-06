#pragma leco tool
import jute;
import silog;
import yoyo;
import what_the_font;

void fail(const char *msg) { throw 0; }

int main() try {
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

  unsigned charid[10240];
  unsigned curid{};
  unsigned char charmap[img_w * img_h]{};
  unsigned px{};
  unsigned py{font_h};
  for (auto line : text) {
    for (auto g : f.shape_pt(line).glyphs()) {
      auto &id = charid[g.codepoint()];
      if (id > 0)
        continue;

      id = ++curid;
      g.blit(charmap, img_w, img_h, px, py);
      px += g.x_advance();
      if (px > img_w) {
        px = 0;
        py += font_h;
      }
    }
  }

  yoyo::file_writer out{"out/result.pgm"};
  out.writef("P2\n%d %d 256\n", img_w, img_h * 2).take(fail);
  for (auto c : img)
    out.writef("%d ", c).take(fail);
  for (auto c : charmap)
    out.writef("%d ", c).take(fail);
} catch (const wtf::ft_error &e) {
  silog::log(silog::error, "Failed with freetype error [%s]", e.what());
}
