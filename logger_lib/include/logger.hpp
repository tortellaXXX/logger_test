#pragma once
#include <string>
#include <chrono>
#include <fstream>
#include <mutex>

// -----------------------------------------------------------------------------
// Уровни важности логов
enum class LogLevel { 
    INFO = 0,     // Информационные сообщения
    WARNING = 1,  // Предупреждения
    ERROR = 2     // Ошибки
};

// -----------------------------------------------------------------------------
// Класс Logger
// Предназначен для записи сообщений в журнал с потокобезопасностью и уровнями важности
class Logger {
public:
    // Конструктор: принимает имя файла журнала и минимальный уровень логирования
    Logger(const std::string& filename, LogLevel level);

    // Деструктор: закрывает файл журнала
    ~Logger();

    // Запись сообщения с указанным уровнем
    void log(const std::string& message, LogLevel level);

    // Запись сообщения с автоматическим определением уровня по префиксу [LEVEL] Текст
    void logWithAutoLevel(const std::string& message);

    // Изменение минимального уровня логирования после инициализации
    void setLogLevel(LogLevel level);

private:
    std::ofstream file;   // файл журнала
    LogLevel minLevel;    // минимальный уровень сообщений, которые будут записаны
    std::mutex mtx;       // мьютекс для потокобезопасной записи

    // Приватная функция: парсинг уровня сообщения из строки
    LogLevel parseLevel(const std::string& message);
};
