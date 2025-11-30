#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <vector>

struct FT_FaceRec_;
using FT_Face = FT_FaceRec_*;

namespace cvtext {

struct TextStyle {
    int font_size = 24;
    cv::Scalar color = cv::Scalar(255, 255, 255);  // BGR
    double line_spacing = 1.2;
    int outline_thickness = 1;
};

class TextRenderer {
public:
    explicit TextRenderer(const std::string& font_path);
    ~TextRenderer();

    // Основной метод: рисует UTF-8 текст на изображении
    bool putText(cv::Mat& img,
                 const std::string& text,
                 cv::Point org,
                 const TextStyle& style = {}) const;

    // Рисует многострочный текст (поддержка \n)
    bool putTextMulti(cv::Mat& img,
                      const std::string& text,
                      cv::Point org,
                      const TextStyle& style = {}) const;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl_;
};

}  // namespace cvtext