cmake_minimum_required(VERSION 3.14)
project(CyrillicRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)
find_package(PkgConfig REQUIRED)

pkg_check_modules(FREETYPE REQUIRED IMPORTED_TARGET freetype2)
pkg_check_modules(HARFBUZZ REQUIRED IMPORTED_TARGET harfbuzz)

add_executable(draw_text main.cpp TextRenderer.cpp)
target_link_libraries(draw_text
    ${OpenCV_LIBS}
    PkgConfig::FREETYPE
    PkgConfig::HARFBUZZ
)
target_include_directories(draw_text PRIVATE .)