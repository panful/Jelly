#include "InteractorGLFW.h"
#include "Renderer.h"
#include "WindowGLFW.h"

#include <iostream>
#include <memory>

int main()
{
    auto renderer = std::make_shared<Jelly::Renderer>();

    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->AddRenderer(renderer);
    window->SetSize(400, 300);
    window->SetTitle("Test window");
    window->Render();

    auto interactor = std::make_shared<Jelly::InteractorGLFW>();
    interactor->SetWindow(window);
    interactor->Start();
}
