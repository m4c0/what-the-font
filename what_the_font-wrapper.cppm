module;
#pragma leco add_impl freetype harfbuzz
#pragma leco add_include_dir "freetype/include"
#include "harfbuzz/src/hb-ft.h"

export module what_the_font:wrapper;

namespace wtf {
using FT_Byte = ::FT_Byte;
using FT_Error = ::FT_Error;
using FT_Face = ::FT_Face;
using FT_Library = ::FT_Library;
using hb_buffer_t = ::hb_buffer_t;
using hb_font_t = ::hb_font_t;
using hb_glyph_info_t = ::hb_glyph_info_t;
using hb_glyph_position_t = ::hb_glyph_position_t;

constexpr const auto ft_load_render = FT_LOAD_RENDER;

constexpr const auto FT_Done_Face = &::FT_Done_Face;
constexpr const auto FT_Done_FreeType = &::FT_Done_FreeType;
constexpr const auto FT_Error_String = &::FT_Error_String;
constexpr const auto FT_Init_FreeType = &::FT_Init_FreeType;
constexpr const auto FT_Load_Glyph = &::FT_Load_Glyph;
constexpr const auto FT_New_Face = &::FT_New_Face;
constexpr const auto FT_New_Memory_Face = &::FT_New_Memory_Face;
constexpr const auto FT_Reference_Face = &::FT_Reference_Face;
constexpr const auto FT_Set_Char_Size = &::FT_Set_Char_Size;
constexpr const auto hb_buffer_add_utf8 = &::hb_buffer_add_utf8;
constexpr const auto hb_buffer_create = &::hb_buffer_create;
constexpr const auto hb_buffer_destroy = &::hb_buffer_destroy;
constexpr const auto hb_buffer_get_length = &::hb_buffer_get_length;
constexpr const auto hb_buffer_get_glyph_infos = &::hb_buffer_get_glyph_infos;
constexpr const auto hb_buffer_get_glyph_positions =
    &::hb_buffer_get_glyph_positions;
constexpr const auto hb_buffer_reference = &::hb_buffer_reference;
constexpr const auto hb_buffer_set_direction = &::hb_buffer_set_direction;
constexpr const auto hb_buffer_set_language = &::hb_buffer_set_language;
constexpr const auto hb_buffer_set_script = &::hb_buffer_set_script;
constexpr const auto hb_font_destroy = &::hb_font_destroy;
constexpr const auto hb_language_from_string = &::hb_language_from_string;
constexpr const auto hb_ft_font_create_referenced =
    &::hb_ft_font_create_referenced;
constexpr const auto hb_shape = &::hb_shape;
} // namespace wtf
