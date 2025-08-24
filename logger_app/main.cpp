#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <locale>
#include "logger.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

// Структура для очереди логов
struct LogMessage {
    std::string text;
};

std::queue<LogMessage> logQueue;
std::mutex queueMutex;
std::condition_variable cv;
bool finished = false;

// Поток для записи логов
void logWorker(Logger& logger) {
    while (true) {
        std::unique_lock<std::mutex> lock(queueMutex);
        cv.wait(lock, []{ return !logQueue.empty() || finished; });

        while (!logQueue.empty()) {
            LogMessage msg = logQueue.front();
            logQueue.pop();
            lock.unlock();

            logger.logWithAutoLevel(msg.text);

            lock.lock();
        }

        if (finished && logQueue.empty()) break;
    }
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // консоль в UTF-8 под Windows
#endif

    std::locale::global(std::locale("")); 
    std::cout << "Тест UTF-8 ✔" << std::endl;

    std::string logFile = (argc > 1) ? argv[1] : "app.log";
    LogLevel defaultLevel = (argc > 2) ? static_cast<LogLevel>(std::stoi(argv[2])) : LogLevel::INFO;

    Logger logger(logFile, defaultLevel);

    std::thread worker(logWorker, std::ref(logger));

    std::string line;
    while (true) {
        std::cout << "Введите сообщение (например [WARNING] Текст) или 'exit' для выхода: ";
        std::getline(std::cin, line);
        if (line == "exit") break;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            logQueue.push({line});
        }
        cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        finished = true;
    }
    cv.notify_one();
    worker.join();

    return 0;
}
