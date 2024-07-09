#include "Renderer.h"
#include "Actor3D.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "FloatData.h"
#include <gtest/gtest.h>
#include <limits>
#include <memory>
#include <vector>

TEST(Test_Renderer, getBoundsNoActor)
{
    auto renderer = std::make_shared<Jelly::Renderer>();

    auto bounds = renderer->GetVisibleActorBounds();

    EXPECT_DOUBLE_EQ(bounds[0], -1.);
    EXPECT_DOUBLE_EQ(bounds[1], 1.);
    EXPECT_DOUBLE_EQ(bounds[2], -1.);
    EXPECT_DOUBLE_EQ(bounds[3], 1.);
    EXPECT_DOUBLE_EQ(bounds[4], -1.);
    EXPECT_DOUBLE_EQ(bounds[5], 1.);
}

TEST(Test_Renderer, getBoundsNoMapper)
{
    auto actor    = std::make_shared<Jelly::Actor3D>();
    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor);

    auto bounds = renderer->GetVisibleActorBounds();

    EXPECT_DOUBLE_EQ(bounds[0], -1.);
    EXPECT_DOUBLE_EQ(bounds[1], 1.);
    EXPECT_DOUBLE_EQ(bounds[2], -1.);
    EXPECT_DOUBLE_EQ(bounds[3], 1.);
    EXPECT_DOUBLE_EQ(bounds[4], -1.);
    EXPECT_DOUBLE_EQ(bounds[5], 1.);
}

TEST(Test_Renderer, getBoundsNoDataSet)
{
    auto mapper = std::make_shared<Jelly::DataSetMapper>();

    auto actor = std::make_shared<Jelly::Actor3D>();
    actor->SetMapper(mapper);

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor);

    auto bounds = renderer->GetVisibleActorBounds();

    EXPECT_DOUBLE_EQ(bounds[0], -1.);
    EXPECT_DOUBLE_EQ(bounds[1], 1.);
    EXPECT_DOUBLE_EQ(bounds[2], -1.);
    EXPECT_DOUBLE_EQ(bounds[3], 1.);
    EXPECT_DOUBLE_EQ(bounds[4], -1.);
    EXPECT_DOUBLE_EQ(bounds[5], 1.);
}

TEST(Test_Renderer, getBoundsSingleActor)
{
    auto f_min = std::numeric_limits<float>::lowest();
    auto f_max = std::numeric_limits<float>::max();

    // clang-format off
    std::vector<float> points {
        -.009f,   -10.2f,   f_min,
         -.01f,      .8f,     1.f,
          -.1f,    10.8f,    -1.f,
           .1f,     -.2f,   f_max,
    };
    // clang-format on

    auto pointData = std::make_shared<Jelly::FloatData>();
    pointData->SetData(std::move(points));

    auto dataSet = std::make_shared<Jelly::DataSet>();
    dataSet->SetPoints(pointData);

    auto mapper = std::make_shared<Jelly::DataSetMapper>();
    mapper->SetDataSet(dataSet);

    auto actor = std::make_shared<Jelly::Actor3D>();
    actor->SetMapper(mapper);

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor);

    auto bounds = renderer->GetVisibleActorBounds();

    static constexpr float abs_error {std::numeric_limits<float>::epsilon()};

    EXPECT_NEAR(static_cast<float>(bounds[0]), -.1f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[1]), .1f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[2]), -10.2f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[3]), 10.8f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[4]), f_min, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[5]), f_max, abs_error);
}

TEST(Test_Renderer, getBoundsMultipleActor)
{
    auto f_min = std::numeric_limits<float>::lowest();
    auto f_max = std::numeric_limits<float>::max();

    // clang-format off
    std::vector<float> points1 {
        -.8f, -.8f,  0.f,
        -.8f,  .2f,  0.f,
         .2f,  .2f,  0.f,
         .2f, -.8f,  0.f,
    };

    std::vector<float> points2 {
        -.2f, -.2f,  .1f,
        -.2f,  .8f,  .1f,
         .8f,  .8f,  .1f,
         .8f, -.2f,  .1f,
    };

    std::vector<float> points3 {
        -.009f,   -10.2f,   f_min,
         -.01f,      .8f,     1.f,
          -.1f,    10.8f,    -1.f,
           .1f,     -.2f,   f_max,
    };
    // clang-format on

    auto pointData1 = std::make_shared<Jelly::FloatData>();
    pointData1->SetData(std::move(points1));

    auto pointData2 = std::make_shared<Jelly::FloatData>();
    pointData2->SetData(std::move(points2));

    auto pointData3 = std::make_shared<Jelly::FloatData>();
    pointData3->SetData(std::move(points3));

    auto dataSet1 = std::make_shared<Jelly::DataSet>();
    dataSet1->SetPoints(pointData1);

    auto dataSet2 = std::make_shared<Jelly::DataSet>();
    dataSet2->SetPoints(pointData2);

    auto dataSet3 = std::make_shared<Jelly::DataSet>();
    dataSet3->SetPoints(pointData3);

    auto mapper1 = std::make_shared<Jelly::DataSetMapper>();
    mapper1->SetDataSet(dataSet1);

    auto mapper2 = std::make_shared<Jelly::DataSetMapper>();
    mapper2->SetDataSet(dataSet2);

    auto mapper3 = std::make_shared<Jelly::DataSetMapper>();
    mapper3->SetDataSet(dataSet3);

    auto actor1 = std::make_shared<Jelly::Actor3D>();
    actor1->SetMapper(mapper1);

    auto actor2 = std::make_shared<Jelly::Actor3D>();
    actor2->SetMapper(mapper2);

    auto actor3 = std::make_shared<Jelly::Actor3D>();
    actor3->SetMapper(mapper3);

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor1);
    renderer->AddActor(actor2);
    renderer->AddActor(actor3);

    auto bounds = renderer->GetVisibleActorBounds();

    static constexpr float abs_error {std::numeric_limits<float>::epsilon()};

    EXPECT_NEAR(static_cast<float>(bounds[0]), -.8f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[1]), .8f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[2]), -10.2f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[3]), 10.8f, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[4]), f_min, abs_error);
    EXPECT_NEAR(static_cast<float>(bounds[5]), f_max, abs_error);
}
