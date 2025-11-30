#include "TextRenderer.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>


int main() {
    try {
        // Выбор шрифта (строгий, без лишних элементов)
        //std::string font_path = "/usr/share/fonts/truetype/noto/NotoSans-Regular.ttf";
        std::string font_path = "/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf";
        std::cout << "✅ Используем шрифт: " << font_path << "\n";

        // Инициализация рендерера текста с выбранным шрифтом
        cvtext::TextRenderer renderer(font_path);

        // Создание изображения для отрисовки текста
        cv::Mat img(600, 1000, CV_8UC3, cv::Scalar(127, 127, 127));

        // Стиль для белого текста
        cvtext::TextStyle style_white;
        style_white.font_size = 50;
        style_white.color = cv::Scalar(255, 255, 255);
        style_white.outline_thickness = 1;

        renderer.putText(img, "Белый текст", cv::Point(50, 100), style_white);

        // Стиль для красного текста
        cvtext::TextStyle style_red;
        style_red.font_size = 60;
        style_red.color = cv::Scalar(0, 0, 255); // красный
        style_red.outline_thickness = 3;

        renderer.putText(img, "Красный текст", cv::Point(50, 200), style_red);

        // Стиль для синего текста
        cvtext::TextStyle style_blue;
        style_blue.font_size = 70;
        style_blue.color = cv::Scalar(255, 0, 0); // синий
        style_blue.outline_thickness = 5;

        renderer.putText(img, "Синий текст", cv::Point(50, 300), style_blue);

        // Сохранение результата в файл
        cv::imwrite("output.png", img);
        std::cout << "✅ Сохранено в output.png\n";

    } catch (const std::exception& e) {
        std::cerr << "❌ Ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}