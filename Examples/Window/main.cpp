#include "Actor3D.h"
#include "InteractorGLFW.h"
#include "Renderer.h"
#include "WindowGLFW.h"
#include "MapperPolyData.h"
#include <iostream>
#include <memory>

int main()
{
    auto mapper = std::make_shared<Jelly::MapperPolyData>();

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