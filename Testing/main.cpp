#include "Environment.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    ::testing::AddGlobalTestEnvironment(new GlobalTestEnvironment());

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}
