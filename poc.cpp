#pragma leco tool
import jute;
import silog;
import yoyo;
import what_the_font;

void fail(jute::view msg) { throw 0; }

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
  auto f = l.new_face(test_font);
  f.set_char_size(font_h);

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

  unsigned charid[10240]; // TODO: max(codepoint) or hashmap
  unsigned curid{};
  unsigned char charmap[img_w * img_h]{};
  unsigned px{};
  unsigned py{};
  for (auto line : text) {
    for (auto g : f.shape_pt(line).glyphs()) {
      auto &id = charid[g.codepoint()];
      if (id > 0)
        continue;

      g.load_glyph();
      auto [x, y, w, h] = g.bitmap_rect();
      if (px + w + 2 > img_w / 2) { // half width forces line break
        px = 0;
        py += font_h; // TODO: max(h + 2)
      }

      id = ++curid;
      g.blit(charmap, img_w, img_h, px - x + 1, py + y + 1);
      px += w + 2;
    }
  }

  auto out = yoyo::file_writer::open("out/result.pgm").take(fail);
  out.writef("P2\n%d %d 256\n", img_w, img_h * 2).take(fail);
  for (auto c : img)
    out.writef("%d ", c).take(fail);
  for (auto c : charmap)
    out.writef("%d ", c).take(fail);
} catch (const wtf::ft_error &e) {
  silog::log(silog::error, "Failed with freetype error [%s]", e.what());
}
