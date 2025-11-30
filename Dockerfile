FROM ubuntu:22.04

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Установка зависимостей
RUN apt update && apt install -y \
    libopencv-dev \
    libfreetype6-dev \
    libharfbuzz-dev \
    build-essential cmake \
    fonts-dejavu-core \
    fonts-dejavu-extra \
    fonts-liberation \
    fonts-noto-core \
    fonts-roboto \
    fonts-open-sans \
    && rm -rf /var/lib/apt/lists/*

# Копируем исходники
WORKDIR /app
COPY . .

# Сборка
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --parallel

# Запуск
CMD ["sh", "-c", "./build/draw_text && cp output.png /data/output.png"]