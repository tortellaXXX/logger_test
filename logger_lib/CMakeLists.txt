# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(logger_lib LANGUAGES CXX)

# -------------------------------------------------------------------
# Создаём статическую и динамическую библиотеки из одного исходного файла
add_library(logger_static STATIC src/logger.cpp)
add_library(logger_shared SHARED src/logger.cpp)

# -------------------------------------------------------------------
# Указываем путь к заголовочным файлам
# Используем абсолютный путь через CMAKE_CURRENT_SOURCE_DIR
target_include_directories(logger_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(logger_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# -------------------------------------------------------------------
# Примечание:
# logger_static и logger_shared собираются отдельно и могут быть использованы
# в приложении logger_app. Папка include содержит logger.hpp.
