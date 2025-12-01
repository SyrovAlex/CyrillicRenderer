# Cyrillic Text Renderer

Библиотека для рендеринга кириллического текста на изображениях с использованием FreeType и OpenCV.

## Особенности

- ✅ Поддержка кириллических символов (UTF-8)
- ✅ Настройка размера, цвета и обводки текста
- ✅ Многострочный текст (поддержка `\n`)
- ✅ Альфа-смешивание для плавного рендеринга

## Зависимости

- OpenCV 4.x
- FreeType 2.x
- HarfBuzz
- C++17

Проверить наличие зависимостей:

```bash
sudo apt update
dpkg -s libopencv-dev
dpkg -s libfreetype6-dev
dpkg -s libharfbuzz-dev
```

Установка зависимостей:

```bash
sudo apt install libopencv-dev libfreetype6-dev libharfbuzz-dev
```

Проверка наличия шрифтов:
```bash
fc-list | grep -i "Open Sans"
fc-list | grep -i "Noto Sans"
fc-list | grep -i "DejaVu Sans"
fc-list | grep -i "Roboto"
fc-list | grep -i "Liberation"
```

Установка шрифтов:
```bash
sudo apt install fonts-open-sans fonts-noto-cjk fonts-dejavu fonts-roboto fonts-liberation
```

Поиск путей к шрифтам:
```bash
fc-list | grep -i "Open Sans"
```

## Сборка

```bash
# Клонирование репозитория
git clone https://github.com/SyrovAlex/CyrillicRenderer
cd CyrillicRenderer

# Сборка проекта
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Использование
Базовый пример
```cpp

#include "TextRenderer.hpp"
#include <opencv2/opencv.hpp>

int main() {
    try {
        // Инициализация рендерера с указанием пути к шрифту
        cvtext::TextRenderer renderer("/path/to/font.ttf");
        
        // Создание изображения
        cv::Mat img(600, 800, CV_8UC3, cv::Scalar(127, 127, 127));
        
        // Настройка стиля текста
        cvtext::TextStyle style;
        style.font_size = 50;
        style.color = cv::Scalar(255, 255, 255);  // Белый цвет (BGR)
        style.outline_thickness = 2;
        style.line_spacing = 1.2;
        
        // Отрисовка текста
        renderer.putText(img, "Привет, мир!", cv::Point(50, 100), style);
        
        // Сохранение результата
        cv::imwrite("output.png", img);
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
```

Многострочный текст
```cpp

cvtext::TextStyle style;
style.font_size = 30;
style.line_spacing = 1.5;

std::string multiline_text = "Первая строка\nВторая строка\nТретья строка";
renderer.putTextMulti(img, multiline_text, cv::Point(50, 200), style);
```

Структура TextStyle
```cpp

struct TextStyle {
    int font_size = 24;           // Размер шрифта в пикселях
    cv::Scalar color = cv::Scalar(255, 255, 255);  // Цвет в формате BGR
    double line_spacing = 1.2;    // Межстрочный интервал
    int outline_thickness = 1;    // Толщина обводки
};
```

Docker сборка

Проект включает Dockerfile для удобной сборки и эксперементов в изолированном окружении:
```bash

# Сборка образа
docker build -t cyrillic-renderer .

# Запуск контейнера
docker run -v $(pwd)/output:/data cyrillic-renderer
```

В Docker-образ включены популярные свободные шрифты:
    Open Sans,
    Noto Sans,
    Roboto,
    DejaVu Sans,
    Liberation Sans
