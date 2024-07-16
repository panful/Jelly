#include "DataSetMapper.h"
#include "Actor3D.h"
#include "DataSet.h"
#include "FloatData.h"
#include "InteractorGLFW.h"
#include "Renderer.h"
#include "WindowGLFW.h"
#include <gtest/gtest.h>
#include <memory>
#include <vector>

namespace {
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

std::vector<float> colors1 {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
    1.f, 1.f, 1.f,
};

std::vector<float> colors2 {
    1.f, 1.f, 0.f,
    1.f, 0.f, 1.f,
    0.f, 1.f, 1.f,
    1.f, 1.f, 1.f,
};

std::vector<uint32_t> triangles {0, 1, 2, 0, 2, 3};
// clang-format on
} // namespace

TEST(Test_DataSetMapper, renderingResult)
{
    auto pointData1 = std::make_shared<Jelly::FloatData>();
    pointData1->SetData(std::move(points1));

    auto pointData2 = std::make_shared<Jelly::FloatData>();
    pointData2->SetData(std::move(points2));

    auto colorData1 = std::make_shared<Jelly::FloatData>();
    colorData1->SetData(std::move(colors1));

    auto colorData2 = std::make_shared<Jelly::FloatData>();
    colorData2->SetData(std::move(colors2));

    auto indexData = std::make_shared<Jelly::DataArrayTemplate<uint32_t>>();
    indexData->SetData(std::move(triangles));

    auto dataSet1 = std::make_shared<Jelly::DataSet>();
    dataSet1->SetPoints(pointData1);
    dataSet1->SetColors(colorData1);
    dataSet1->SetIndices(indexData);

    auto dataSet2 = std::make_shared<Jelly::DataSet>();
    dataSet2->SetPoints(pointData2);
    dataSet2->SetColors(colorData2);
    dataSet2->SetIndices(indexData);

    auto mapper1 = std::make_shared<Jelly::DataSetMapper>();
    mapper1->SetDataSet(dataSet1);
    mapper1->SetColor({1.f, 0.f, 1.f});
    mapper1->SetColorMode(Jelly::ColorMode::VertexColoring);
    mapper1->SetEnableLighting(false);

    auto mapper2 = std::make_shared<Jelly::DataSetMapper>();
    mapper2->SetDataSet(dataSet2);
    mapper2->SetColor({1.f, 1.f, 0.f});
    mapper2->SetColorMode(Jelly::ColorMode::UniformColor);
    mapper2->SetEnableLighting(false);

    auto actor1 = std::make_shared<Jelly::Actor3D>();
    actor1->SetMapper(mapper1);

    auto actor2 = std::make_shared<Jelly::Actor3D>();
    actor2->SetMapper(mapper2);

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor1);
    renderer->AddActor(actor2);
    renderer->SetBackground({.2f, .2f, .2f, 1.f});

    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->SetEnableGetRenderingResult(true);
    window->AddRenderer(renderer);
    window->SetSize(400, 400);
    window->SetTitle("Test window");
    window->Render();

    auto result = window->GetLastRenderingResult();
    EXPECT_EQ(result.size(), 400 * 400 * 4);

    auto expect_pixels = [&result](uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        auto index = (y * 400 + x) * 4;
        EXPECT_EQ(result[index + 0], r);
        EXPECT_EQ(result[index + 1], g);
        EXPECT_EQ(result[index + 2], b);
        EXPECT_EQ(result[index + 3], a);
    };

    expect_pixels(10, 10, 51, 51, 51, 255);
    expect_pixels(50, 50, 242, 0, 13, 255);
    expect_pixels(100, 100, 178, 0, 77, 255);
    expect_pixels(150, 150, 114, 0, 141, 255);
    expect_pixels(200, 200, 50, 0, 205, 255);
    expect_pixels(250, 250, 255, 255, 0, 255);
    expect_pixels(300, 300, 255, 255, 0, 255);
    expect_pixels(350, 350, 255, 255, 0, 255);
    expect_pixels(390, 390, 51, 51, 51, 255);
    expect_pixels(50, 350, 51, 51, 51, 255);
    expect_pixels(150, 250, 51, 51, 51, 255);
    expect_pixels(250, 150, 51, 51, 51, 255);
    expect_pixels(350, 50, 51, 51, 51, 255);
}
