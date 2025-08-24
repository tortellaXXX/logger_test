#include "logger.hpp"
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream> // для вывода ошибок при открытии файла

// -----------------------------------------------------------------------------
// Конструктор логгера
Logger::Logger(const std::string& filename, LogLevel level)
    : minLevel(level)
{
    // Открываем файл в режиме добавления
    file.open(filename, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл лога: " << filename << std::endl;
    }
}

// -----------------------------------------------------------------------------
// Деструктор
Logger::~Logger() {
    if (file.is_open()) file.close();
}

// -----------------------------------------------------------------------------
// Установка минимального уровня логирования
void Logger::setLogLevel(LogLevel level) {
    minLevel = level;
}

// -----------------------------------------------------------------------------
// Запись сообщения с заданным уровнем
void Logger::log(const std::string& message, LogLevel level) {
    // Не логируем, если уровень ниже минимального или файл не открыт
    if (level < minLevel || !file.is_open()) return;

    std::lock_guard<std::mutex> lock(mtx); // потокобезопасность

    // Время текущего сообщения
    auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file << "[" << std::put_time(std::localtime(&t), "%F %T") << "] ";

    // Метка уровня
    switch(level) {
        case LogLevel::INFO:    file << "[INFO] ";    break;
        case LogLevel::WARNING: file << "[WARNING] "; break;
        case LogLevel::ERROR:   file << "[ERROR] ";   break;
    }

    file << message << std::endl;
}

// -----------------------------------------------------------------------------
// Приватная функция: парсинг уровня из строки формата [LEVEL] Текст
LogLevel Logger::parseLevel(const std::string& message) {
    if (message.size() > 9 && message[0] == '[') {
        size_t end = message.find(']');
        if (end != std::string::npos) {
            std::string lvl = message.substr(1, end - 1);
            std::transform(lvl.begin(), lvl.end(), lvl.begin(), ::toupper);

            if (lvl == "INFO")    return LogLevel::INFO;
            if (lvl == "WARNING") return LogLevel::WARNING;
            if (lvl == "ERROR")   return LogLevel::ERROR;
        }
    }

    // Если уровень не указан, возвращаем INFO по умолчанию
    return LogLevel::INFO;
}

// -----------------------------------------------------------------------------
// Логирование с автоопределением уровня по префиксу [LEVEL] Текст
void Logger::logWithAutoLevel(const std::string& message) {
    LogLevel lvl = parseLevel(message);  // определяем уровень
    std::string msgCopy = message;

    // Отрезаем префикс уровня, чтобы в логе остался только текст
    size_t pos = msgCopy.find(']');
    if (pos != std::string::npos && msgCopy[0] == '[') {
        msgCopy = msgCopy.substr(pos + 1);
        // удаляем возможные пробелы в начале
        while (!msgCopy.empty() && std::isspace(msgCopy[0])) msgCopy.erase(0, 1);
    }

    log(msgCopy, lvl); // вызываем обычное логирование
}
