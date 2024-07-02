#include "FloatData.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

TEST(Test_FloatData, resize)
{
    Jelly::FloatData data {};
    data.Resize(10);

    EXPECT_EQ(data.GetElementCount(), 10);
}

TEST(Test_FloatData, setData_getData)
{
    std::vector<float> vec {1.f, 2.f, 3.f, 1.5f, 2.5f, 3.5f};

    Jelly::FloatData data {};
    data.SetData(vec);

    EXPECT_EQ(data.GetElementCount(), static_cast<uint32_t>(vec.size()));
    EXPECT_EQ(data.GetData(), vec);
}

TEST(Test_FloatData, setOtherTypeData_getData)
{
    std::vector<double> vec {1., 1.5, 3.};

    Jelly::FloatData data {};
    data.SetData(vec);

    EXPECT_EQ(data.GetElementCount(), static_cast<uint32_t>(vec.size()));

    for (size_t i = 0; i < vec.size(); ++i)
    {
        EXPECT_FLOAT_EQ(data.GetData()[i], static_cast<float>(vec[i]));
    }

    std::vector<uint8_t> vec2 {1, 2, 4, 8, 16, 32, 64, 128};
    data.SetData(vec2);

    EXPECT_EQ(data.GetElementCount(), static_cast<uint32_t>(vec2.size()));

    for (size_t i = 0; i < vec2.size(); ++i)
    {
        EXPECT_FLOAT_EQ(data.GetData()[i], static_cast<float>(vec2[i]));
    }
}

TEST(Test_FloatData, setDataPointer)
{
    float f_ptr[] = {1.f, 2.f, 3.f, 1.5f, 2.5f, 3.5f};
    uint32_t size = sizeof(f_ptr) / sizeof(f_ptr[0]);

    Jelly::FloatData data {};
    data.SetData(f_ptr, size);

    EXPECT_EQ(data.GetElementCount(), size);
    for (uint32_t i = 0; i < size; ++i)
    {
        EXPECT_FLOAT_EQ(f_ptr[i], data.GetData()[i]);
    }

    float f_ptr2[] = {4.4f, 5.5f, 6.6f, 7.7f};
    uint32_t size2 = sizeof(f_ptr2) / sizeof(f_ptr2[0]);

    data.SetData(f_ptr2, size2);

    EXPECT_EQ(data.GetElementCount(), size2);
    for (uint32_t i = 0; i < size2; ++i)
    {
        EXPECT_FLOAT_EQ(f_ptr2[i], data.GetData()[i]);
    }
}

TEST(Test_FloatData, getPointer)
{
    std::vector<float> vec {1.f, 2.f, 3.f, 1.5f, 2.5f, 3.5f};

    Jelly::FloatData data {};
    data.SetData(vec);

    EXPECT_EQ(data.GetElementCount(), static_cast<uint32_t>(vec.size()));

    auto ptr = data.GetPointer();
    for (size_t i = 0; i < vec.size(); ++i)
    {
        EXPECT_FLOAT_EQ(vec[i], *(ptr++));
    }
}

TEST(Test_FloatData, getDataType_getDataTypeSize)
{
    std::vector<float> vec {1.f, 2.f, 3.f, 1.5f, 2.5f, 3.5f};

    Jelly::FloatData data {};
    data.SetData(vec);

    EXPECT_EQ(data.GetDataType(), Jelly::DataType::Float);
    EXPECT_EQ(data.GetDataTypeSize(), sizeof(float));
}

TEST(Test_FloatData, polymorphism)
{
    std::vector<float> vec {1.f, 2.f, 3.f, 1.5f, 2.5f, 3.5f};

    std::unique_ptr<Jelly::FloatData> f_data = std::make_unique<Jelly::FloatData>();
    f_data->SetData(vec);

    std::unique_ptr<Jelly::DataArray> data = std::move(f_data);

    EXPECT_EQ(Jelly::DataType::Float, data->GetDataType());
    EXPECT_EQ(static_cast<uint32_t>(sizeof(float)), data->GetDataTypeSize());
    EXPECT_EQ(static_cast<uint32_t>(vec.size()), data->GetElementCount());

    auto ptr = reinterpret_cast<const float*>(data->GetVoidPointer());
    for (size_t i = 0; i < vec.size(); ++i)
    {
        EXPECT_FLOAT_EQ(vec[i], *(ptr++));
    }
}
