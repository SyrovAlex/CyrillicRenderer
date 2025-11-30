#include "TextRenderer.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <stdexcept>
#include <cmath>

namespace cvtext {

struct TextRenderer::Impl {
    FT_Library ft_lib = nullptr;
    FT_Face face = nullptr;
    std::string font_path;

    Impl(const std::string& path) : font_path(path) {
        if (FT_Init_FreeType(&ft_lib))
            throw std::runtime_error("Failed to initialize FreeType");

        if (FT_New_Face(ft_lib, font_path.c_str(), 0, &face))
            throw std::runtime_error("Failed to load font: " + font_path);
    }

    ~Impl() {
        if (face) FT_Done_Face(face);
        if (ft_lib) FT_Done_FreeType(ft_lib);
    }

    bool renderGlyph(cv::Mat& img, FT_Bitmap& bitmap, int x, int y,
                     const cv::Scalar& color,
                     int outline_thickness) const {
        if (bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) return false;

        for (int by = 0; by < bitmap.rows; ++by) {
            for (int bx = 0; bx < bitmap.width; ++bx) {
                int img_x = x + bx;
                int img_y = y + by;
                if (img_x < 0 || img_y < 0 || img_x >= img.cols || img_y >= img.rows)
                    continue;

                uint8_t alpha = bitmap.buffer[by * bitmap.pitch + bx];
                if (alpha == 0) continue;

                float a = alpha / 255.0f;

                for (int dy = -outline_thickness; dy <= outline_thickness; ++dy) {
                    for (int dx = -outline_thickness; dx <= outline_thickness; ++dx) {
                        if (dx == 0 && dy == 0) continue;
                        int ox = img_x + dx, oy = img_y + dy;
                        if (ox < 0 || oy < 0 || ox >= img.cols || oy >= img.rows) continue;
                        cv::Vec3b& p = img.at<cv::Vec3b>(oy, ox);
                        p[0] = static_cast<uchar>(color[0] * a + p[0] * (1 - a));
                        p[1] = static_cast<uchar>(color[1] * a + p[1] * (1 - a));
                        p[2] = static_cast<uchar>(color[2] * a + p[2] * (1 - a));
                    }
                }

                cv::Vec3b& pixel = img.at<cv::Vec3b>(img_y, img_x);
                pixel[0] = static_cast<uchar>(color[0] * a + pixel[0] * (1 - a));
                pixel[1] = static_cast<uchar>(color[1] * a + pixel[1] * (1 - a));
                pixel[2] = static_cast<uchar>(color[2] * a + pixel[2] * (1 - a));
            }
        }
        return true;
    }

    bool layoutAndRender(cv::Mat& img, const std::string& text, cv::Point pos,
                         int font_size, const cv::Scalar& color,
                         int outline_thickness) const {
        if (FT_Set_Pixel_Sizes(face, 0, font_size))
            return false;

        for (size_t i = 0; i < text.size(); ) {
            uint32_t cp;
            uint8_t c = text[i];
            if (c < 0x80) { cp = c; i++; }
            else if ((c & 0xE0) == 0xC0) { cp = ((c & 0x1F) << 6) | (text[i+1] & 0x3F); i += 2; }
            else if ((c & 0xF0) == 0xE0) { cp = ((c & 0x0F) << 12) | ((text[i+1] & 0x3F) << 6) | (text[i+2] & 0x3F); i += 3; }
            else { i++; continue; }

            if (FT_Load_Char(face, cp, FT_LOAD_RENDER)) continue;

            FT_Bitmap& bitmap = face->glyph->bitmap;
            int x = pos.x + face->glyph->bitmap_left;
            int y = pos.y - face->glyph->bitmap_top;

            renderGlyph(img, bitmap, x, y, color, outline_thickness);

            pos.x += face->glyph->advance.x >> 6;
        }
        return true;
    }
};

TextRenderer::TextRenderer(const std::string& font_path)
    : pimpl_(std::make_unique<Impl>(font_path)) {}

TextRenderer::~TextRenderer() = default;

bool TextRenderer::putText(cv::Mat& img, const std::string& text, cv::Point org,
                           const TextStyle& style) const {
    if (text.empty() || img.empty()) return false;
    return pimpl_->layoutAndRender(img, text, org, style.font_size,
                                   style.color,
                                   style.outline_thickness);
}

bool TextRenderer::putTextMulti(cv::Mat& img, const std::string& text, cv::Point org,
                                const TextStyle& style) const {
    if (text.empty() || img.empty()) return false;

    size_t start = 0;
    cv::Point pos = org;

    while (start < text.size()) {
        size_t end = text.find('\n', start);
        if (end == std::string::npos) end = text.size();

        std::string line = text.substr(start, end - start);
        if (!line.empty()) {
            putText(img, line, pos, style);
        }

        start = end + 1;
        pos.y += static_cast<int>(style.font_size * style.line_spacing);
    }
    return true;
}

}  // namespace cvtext