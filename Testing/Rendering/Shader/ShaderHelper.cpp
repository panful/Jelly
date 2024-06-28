#include "ShaderHelper.h"
#include <fstream>
#include <gtest/gtest.h>
#include <string>

using namespace std::string_literals;

static std::string ReadGLSL(const std::string_view fileName)
{
    std::ifstream file(fileName.data());

    EXPECT_TRUE(file.is_open());

    std::istreambuf_iterator<char> beg(file), end;

    return std::string(beg, end);
}

static std::vector<uint32_t> ReadSPV(const std::string_view fileName)
{
    std::ifstream ifs(fileName.data());
    std::string line {};
    std::vector<uint32_t> spv {};
    while (std::getline(ifs, line))
    {
        spv.emplace_back(std::stoul(line));
    }
    return spv;
}

TEST(Test_ShaderHelper, vertexShader)
{
    auto read_spv  = ReadSPV(TEST_DATA_DIR + "/vert.spv"s);
    auto stage_str = ReadGLSL(TEST_DATA_DIR + "/base.vert"s);

    std::optional<std::vector<uint32_t>> generate_spv =
        Jelly::ShaderHelper::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eVertex, stage_str);

    EXPECT_TRUE(generate_spv.has_value());
    EXPECT_TRUE((read_spv == generate_spv.value_or(std::vector<uint32_t>())));
}

TEST(Test_ShaderHelper, fragmenShader)
{
    auto read_spv  = ReadSPV(TEST_DATA_DIR + "/frag.spv"s);
    auto stage_str = ReadGLSL(TEST_DATA_DIR + "/base.frag"s);

    std::optional<std::vector<uint32_t>> generate_spv =
        Jelly::ShaderHelper::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eFragment, stage_str);

    EXPECT_TRUE(generate_spv.has_value());
    EXPECT_TRUE((read_spv == generate_spv.value_or(std::vector<uint32_t>())));
}
