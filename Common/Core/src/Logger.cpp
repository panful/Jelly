#include "Logger.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/spdlog.h"
#include <iostream>
#include <thread>

using namespace Jelly;

Logger::Logger() noexcept
    : m_spdLogger(spdlog::daily_logger_mt("daily_logger", s_fileName, 0, 0))
{
    decltype(spdlog::level::trace) level {};
    switch (s_level)
    {
        case LogLevel::Trace:
            level = spdlog::level::trace;
            break;
        case LogLevel::Debug:
            level = spdlog::level::debug;
            break;
        case LogLevel::Warn:
            level = spdlog::level::warn;
            break;
        case LogLevel::Error:
            level = spdlog::level::err;
            break;
    }

    m_spdLogger->set_level(level);
    m_spdLogger->set_pattern("[%H:%M:%S %z] [%l] <%t> %v");
}

Logger::~Logger() noexcept
{
    Trace("spdlog terminate");
    m_spdLogger->flush();
    spdlog::drop_all();
}

void Logger::SetLevel(const LogLevel level)
{
    s_level = level;
}

void Logger::SetFileName(const std::string_view fileName)
{
    s_fileName = fileName;
}

Logger* Logger::GetInstance() noexcept
{
    static Logger logger {};
    return &logger;
}

void Logger::Trace(const std::string_view message, const std::source_location location) noexcept
{
    m_spdLogger->trace("message: {}, function: {}", message, location.function_name());
}

void Logger::Debug(const std::string_view message, const std::source_location location) noexcept
{
    m_spdLogger->debug("message: {}, function: {}", message, location.function_name());
}

void Logger::Warn(const std::string_view message, const std::source_location location) noexcept
{
    m_spdLogger->warn("message: {}, function: {}", message, location.function_name());
}

void Logger::Error(const std::string_view message, const std::source_location location) noexcept
{
    m_spdLogger->error("message: {}, function: {}", message, location.function_name());
}
