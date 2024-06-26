/**
 * @file Logger.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-17
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#pragma warning(disable : 4251)

#include "GlobalExport.h"
#include "Object.h"
#include <memory>
#include <source_location>
#include <string_view>

namespace spdlog {
class logger;
} // namespace spdlog

namespace Jelly {
enum class LogLevel : uint8_t
{
    Trace,
    Debug,
    Warn,
    Error,
};

class JELLY_EXPORT Logger : public Object
{
private:
    Logger() noexcept;
    ~Logger() noexcept override;

public:
    static Logger* GetInstance() noexcept;
    static void SetLevel(const LogLevel level);
    static void SetFileName(const std::string_view fileName);

public:
    void Trace(
        const std::string_view message = "", const std::source_location location = std::source_location::current()
    ) noexcept;

    void Debug(
        const std::string_view message = "", const std::source_location location = std::source_location::current()
    ) noexcept;

    void Warn(
        const std::string_view message = "", const std::source_location location = std::source_location::current()
    ) noexcept;

    void Error(
        const std::string_view message = "", const std::source_location location = std::source_location::current()
    ) noexcept;

private:
    std::shared_ptr<spdlog::logger> m_spdLogger {};

    inline static std::string s_fileName {"Logs/Jelly.log"};
    inline static LogLevel s_level {LogLevel::Trace};
};
} // namespace Jelly
