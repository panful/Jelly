#include "Actor3D.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "FloatData.h"
#include "InteractorQT.h"
#include "Renderer.h"
#include "WindowQT.h"
#include <QGuiApplication>
#include <iostream>
#include <memory>
#include <vector>

namespace {
// clang-format off
std::vector<float> points {
    -10.f,  -10.f,  -20.f,
    -10.f,   10.f,  -20.f,
     10.f,   10.f,  -20.f,
     10.f,  -10.f,  -20.f,

    -10.f,  -10.f,   20.f,
    -10.f,   10.f,   20.f,
     10.f,   10.f,   20.f,
     10.f,  -10.f,   20.f,
};

std::vector<float> colors {
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

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    auto pointData = std::make_shared<Jelly::FloatData>();
    pointData->SetData(std::move(points));

    auto colorData = std::make_shared<Jelly::FloatData>();
    colorData->SetData(std::move(colors));

    auto indexData = std::make_shared<Jelly::DataArrayTemplate<uint32_t>>();
    indexData->SetData(std::move(triangles));

    auto dataSet = std::make_shared<Jelly::DataSet>();
    dataSet->SetPoints(pointData);
    dataSet->SetColors(colorData);
    dataSet->SetIndices(indexData);

    auto mapper = std::make_shared<Jelly::DataSetMapper>();
    mapper->SetDataSet(dataSet);
    mapper->SetColorMode(Jelly::ColorMode::Vertex);

    auto actor = std::make_shared<Jelly::Actor3D>();
    actor->SetMapper(mapper);
    actor->SetColor({1.f, 0.f, 1.f});

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor);
    renderer->ResetCamera();

    auto window = std::make_shared<Jelly::WindowQT>();
    window->AddRenderer(renderer);
    window->resize(800, 600);
    window->show();

    auto interactor = std::make_shared<Jelly::InteractorQT>();
    interactor->SetWindow(window);
    interactor->Start();

    return app.exec();
}
