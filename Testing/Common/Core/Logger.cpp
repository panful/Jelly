#include "Environment.hpp"
#include "Logger.h"
#include <filesystem>
#include <gtest/gtest.h>

TEST(Test_Logger, createLogFile)
{
    Jelly::Logger::GetInstance()->Trace("test text");

    EXPECT_TRUE(std::filesystem::exists(GlobalTestEnvironment::logFileName));
}
