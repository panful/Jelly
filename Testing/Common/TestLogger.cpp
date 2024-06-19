#include "Logger.h"
#include <chrono>
#include <filesystem>
#include <format>
#include <gtest/gtest.h>
#include <fstream>

TEST(Test_Logger, createLogFile)
{
    auto nowTimePoint   = std::chrono::system_clock::now();
    auto expectFileName = "unitTestOut/Log/createLogFile_" + std::format("{:%Y-%m-%d}", nowTimePoint) + ".log";

    if (std::filesystem::exists(expectFileName))
    {
        std::filesystem::remove(expectFileName);
    }

    EXPECT_FALSE(std::filesystem::exists(expectFileName));

    Jelly::Logger::SetFileName("unitTestOut/Log/createLogFile.log");
    Jelly::Logger::SetLevel(Jelly::LogLevel::Trace);

    Jelly::Logger::GetInstance()->Trace("test text");

    EXPECT_TRUE(std::filesystem::exists(expectFileName));
}
