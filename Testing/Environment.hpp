/**
 * @file Environment.hpp
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Logger.h"
#include <chrono>
#include <filesystem>
#include <format>
#include <gtest/gtest.h>

class GlobalTestEnvironment : public ::testing::Environment
{
public:
    inline static std::string logFileName {};

    void SetUp() override
    {
        auto nowTimePoint = std::chrono::system_clock::now();

        logFileName = "unitTestOut/Log/createLogFile_" + std::format("{:%Y-%m-%d}", nowTimePoint) + ".log";

        if (std::filesystem::exists(logFileName))
        {
            std::filesystem::remove(logFileName);
        }

        Jelly::Logger::SetFileName("unitTestOut/Log/createLogFile.log");
        Jelly::Logger::SetLevel(Jelly::LogLevel::Trace);
    }
};
