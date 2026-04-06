#pragma once
#include <mutex>
#include <sstream>
#include <fstream>
#include <windows.h>
#include <cstdio>
#include <memory>
#pragma warning(disable: 4005)
#define LOG_INFO(...)
#define LOG_ERROR(...)
#include "Includes.h"

using namespace Il2CppBase::Globals; // Your cheat name

namespace Il2CppBase // Your cheat name
{
    class Logger
    {
    public:
        enum class Level : uint8_t
        {
            Call,
            Debug,
            Info,
            Warning,
            Error,
            Hooks
        };
        explicit Logger(Level minLevel = Level::Call);
        ~Logger();

        template<typename... Args>
        void Log(const Level level, Args&&... args)
        {
            std::string message = FormatMessage(std::forward<Args>(args)...);
            ActualLog(level, message);
        }

        void ActualLog(Level level, std::string_view message);

    private:
        const Level MinLevel;
        bool ConsoleExists;
        HANDLE HConsole;
        std::mutex LogMutex;
        std::ofstream FileOut;
        std::string LogFilePath;
        static constexpr std::string_view LevelToString(Level level);
        static constexpr WORD LevelToColor(Level level);
        static std::string GetTimestamp();
        bool InitializeLogDirectory();
        FILE* StdoutFile;
        FILE* StderrFile;

        static bool ContainsPrintfFormat(const std::string& format);

        template<typename T>
        static std::string ConvertToString(T&& arg)
        {
            std::ostringstream ss;
            ss << std::forward<T>(arg);
            return ss.str();
        }

        template<typename... Args>
        static std::string FormatMessage(const char* format, Args&&... args)
        {
            std::string formatStr(format);

            if (ContainsPrintfFormat(formatStr))
                return PrintfFormat(format, std::forward<Args>(args)...);
            else
                return ConcatenateFormat(format, std::forward<Args>(args)...);
        }

        template<typename T>
        static std::string FormatMessage(T&& arg)
        {
            return ConvertToString(std::forward<T>(arg));
        }

        template<typename... Args>
        static std::string PrintfFormat(const char* format, Args&&... args)
        {
            int size = std::snprintf(nullptr, 0, format, std::forward<Args>(args)...);
            if (size < 0) return std::string(format);

            std::string result(size + 1, '\0');
            std::snprintf(&result[0], size + 1, format, std::forward<Args>(args)...);
            result.pop_back();
            return result;
        }

        template<typename First, typename... Rest>
        static std::string ConcatenateFormat(First&& first, Rest&&... rest)
        {
            std::ostringstream ss;
            ss << std::forward<First>(first);

            if constexpr (sizeof...(rest) > 0)
                ((ss << " " << std::forward<Rest>(rest)), ...);

            return ss.str();
        }
    };

    extern Logger* logger;
}

// Replace all Il2CppBase on your cheat name
#define LOG_CALL(...) \
    if (Il2CppBase::logger && IsCalledLogs) \
        Il2CppBase::logger->Log(Il2CppBase::Logger::Level::Call, __VA_ARGS__)
#define LOG_DEBUG(...) \
    if (Il2CppBase::logger && IsDebugging && IsCalledLogs) \
        Il2CppBase::logger->Log(Il2CppBase::Logger::Level::Debug, __VA_ARGS__)
#define LOG_INFO(...) \
    if (Il2CppBase::logger) \
        Il2CppBase::logger->Log(Il2CppBase::Logger::Level::Info, __VA_ARGS__)
#define LOG_HOOKS(...) \
    if (Il2CppBase::logger && IsDebugging) \
        Il2CppBase::logger->Log(Il2CppBase::Logger::Level::Hooks, __VA_ARGS__)
#define LOG_WARN(...) \
    if (Il2CppBase::logger) \
        Il2CppBase::logger->Log(Il2CppBase::Logger::Level::Warning, __VA_ARGS__)
#define LOG_ERROR(...) \
    if (Il2CppBase::logger) \
        Il2CppBase::logger->Log(Il2CppBase::Logger::Level::Error, __VA_ARGS__)
#define LOG_CALL_UPDATE(...) \
    if (Il2CppBase::logger && IsDebugging && IsUpdateCalledLogs) \
        Il2CppBase::logger->Log(Il2CppBase::Logger::Level::Call, __VA_ARGS__)