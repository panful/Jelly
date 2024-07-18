#include "Actor3D.h"
#include "Camera.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "FloatData.h"
#include "InteractorGLFW.h"
#include "InteractorStyleTrackballCamera.h"
#include "Renderer.h"
#include "WindowGLFW.h"
#include <iostream>
#include <memory>
#include <vector>

namespace {
// clang-format off
std::vector<float> points1 {
    -3.f, -3.f, -3.f,
    -3.f,  1.f, -3.f,
     1.f,  1.f, -3.f,
     1.f, -3.f, -3.f,
};

std::vector<float> points2 {
    -1.f, -1.f,  1.f,
    -1.f,  5.f,  1.f,
     3.f,  5.f,  1.f,
     3.f, -1.f,  1.f,
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

int main()
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
    mapper1->SetColorMode(Jelly::ColorMode::Vertex);

    auto mapper2 = std::make_shared<Jelly::DataSetMapper>();
    mapper2->SetDataSet(dataSet2);
    mapper2->SetColorMode(Jelly::ColorMode::Uniform);

    auto actor1 = std::make_shared<Jelly::Actor3D>();
    actor1->SetMapper(mapper1);
    actor1->SetColor({1.f, 0.f, 1.f});

    auto actor2 = std::make_shared<Jelly::Actor3D>();
    actor2->SetMapper(mapper2);
    actor2->SetColor({1.f, 1.f, 0.f});

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor1);
    renderer->AddActor(actor2);
    renderer->GetCamera()->SetCameraType(Jelly::CameraType::Orthographic);
    renderer->ResetCamera();

    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->AddRenderer(renderer);
    window->SetSize(800, 600);
    window->SetTitle("Test window");
    window->Render();

    auto style = std::make_shared<Jelly::InteractorStyleTrackballCamera>();

    auto interactor = std::make_shared<Jelly::InteractorGLFW>();
    interactor->SetInteractorStyle(style);
    interactor->SetWindow(window);
    interactor->Start();
}
