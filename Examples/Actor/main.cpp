#include "Actor3D.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "FloatData.h"
#include "InteractorGLFW.h"
#include "Renderer.h"
#include "WindowGLFW.h"
#include <iostream>
#include <memory>
#include <vector>

namespace {
// clang-format off
std::vector<float> points1 {
    -.0001f, -.0002f,   0.f,
    -.0001f,   100.f,   0.f,
      100.f,   100.f,   0.f,
      100.f, -.0002f,   0.f,

    -.0001f, -.0002f, 500.f,
    -.0001f,   100.f, 500.f,
      100.f,   100.f, 500.f,
      100.f, -.0002f, 500.f,
};

std::vector<float> colors1 {
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f,
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 1.f, 0.f,
};

std::vector<uint32_t> triangles {
    0, 1, 2, 0, 2, 3,
    3, 2, 6, 3, 6, 7,
    7, 6, 5, 7, 5, 4,
    4, 5, 1, 4, 1, 0,
    4, 0, 3, 4, 3, 7,
    1, 5, 6, 1, 6, 2,
};
// clang-format on
} // namespace

int main()
{
    auto pointData1 = std::make_shared<Jelly::FloatData>();
    pointData1->SetData(std::move(points1));

    auto colorData1 = std::make_shared<Jelly::FloatData>();
    colorData1->SetData(std::move(colors1));

    auto indexData = std::make_shared<Jelly::DataArrayTemplate<uint32_t>>();
    indexData->SetData(std::move(triangles));

    auto dataSet1 = std::make_shared<Jelly::DataSet>();
    dataSet1->SetPoints(pointData1);
    dataSet1->SetColors(colorData1);
    dataSet1->SetIndices(indexData);

    auto mapper1 = std::make_shared<Jelly::DataSetMapper>();
    mapper1->SetDataSet(dataSet1);
    mapper1->SetColorMode(Jelly::ColorMode::Vertex);

    auto actor1 = std::make_shared<Jelly::Actor3D>();
    actor1->SetMapper(mapper1);
    actor1->SetColor({1.f, 0.f, 1.f});

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor1);
    renderer->GetCamera()->SetCameraType(Jelly::CameraType::Perspective);
    renderer->ResetCamera();

    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->AddRenderer(renderer);
    window->SetSize(800, 600);
    window->SetTitle("Test window");
    window->Render();

    auto interactor = std::make_shared<Jelly::InteractorGLFW>();
    interactor->SetWindow(window);
    interactor->Start();
}
