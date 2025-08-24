# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(logger_app LANGUAGES CXX)

# -------------------------------------------------------------------
# Создаём исполняемый файл для консольного приложения
add_executable(logger_app main.cpp)

# -------------------------------------------------------------------
# Подключаем статическую библиотеку Logger
# logger_static создаётся в logger_lib/CMakeLists.txt
target_link_libraries(logger_app PRIVATE logger_static)

# -------------------------------------------------------------------
# Указываем папку с заголовочными файлами библиотеки
target_include_directories(logger_app PRIVATE ../logger_lib/include)
