#include "Actor3D.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "InteractorGLFW.h"
#include "Renderer.h"
#include "WindowGLFW.h"

#include <iostream>
#include <memory>

int main()
{
    std::vector<Jelly::Point> points {
        {-.5f, -.5f, 0.f},
        {.5f,  -.5f, 0.f},
        {0.f,  0.f,  0.f},
        {-.5f, .5f,  0.f},
        {.5f,  .5f,  0.f},
    };
    std::vector<uint32_t> triangles {0, 2, 1, 2, 3, 4};

    auto dataSet = std::make_shared<Jelly::DataSet>();
    dataSet->SetPoints(std::move(points));
    dataSet->SetIndices(std::move(triangles));

    auto mapper = std::make_shared<Jelly::DataSetMapper>();
    mapper->SetDataSet(dataSet);

    auto actor = std::make_shared<Jelly::Actor3D>();
    actor->SetMapper(mapper);

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor);

    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->AddRenderer(renderer);
    window->SetSize(400, 300);
    window->SetTitle("Test window");
    window->Render();

    auto interactor = std::make_unique<Jelly::InteractorGLFW>();
    interactor->SetWindow(window);
    interactor->Start();
}
