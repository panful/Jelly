#include "DoubleData.h"
#include <gtest/gtest.h>
#include <vector>

TEST(Test_DoubleData, resize)
{
    Jelly::DoubleData data {};
    data.Resize(10);

    EXPECT_EQ(data.GetElementCount(), 10);
}

TEST(Test_DoubleData, setData_getData)
{
    std::vector<double> vec {1., 2., 3., 1.5, 2.5, 3.5};

    Jelly::DoubleData data {};
    data.SetData(vec);

    EXPECT_EQ(data.GetElementCount(), static_cast<uint32_t>(vec.size()));
    EXPECT_EQ(data.GetData(), vec);
}

TEST(Test_DoubleData, setOtherTypeData_getData)
{
    std::vector<float> vec {1.f, 1.5f, 3.f};

    Jelly::DoubleData data {};
    data.SetData(vec);

    EXPECT_EQ(data.GetElementCount(), static_cast<uint32_t>(vec.size()));

    for (size_t i = 0; i < vec.size(); ++i)
    {
        EXPECT_DOUBLE_EQ(data.GetData()[i], static_cast<double>(vec[i]));
    }

    std::vector<uint8_t> vec2 {1, 2, 4, 8, 16, 32, 64, 128};
    data.SetData(vec2);

    EXPECT_EQ(data.GetElementCount(), static_cast<uint32_t>(vec2.size()));

    for (size_t i = 0; i < vec2.size(); ++i)
    {
        EXPECT_DOUBLE_EQ(data.GetData()[i], static_cast<double>(vec2[i]));
    }
}
