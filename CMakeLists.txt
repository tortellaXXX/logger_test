# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта и язык
project(LoggerProject LANGUAGES CXX)

# Устанавливаем стандарт C++17 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------
# Подключаем библиотеку Logger
# logger_lib содержит CMakeLists.txt, который создаёт статическую и динамическую библиотеку
add_subdirectory(logger_lib)

# -------------------------------------------------------------------
# Подключаем консольное приложение
# logger_app создаёт исполняемый файл и подключает библиотеку logger_static
add_subdirectory(logger_app)
