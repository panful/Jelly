#include "InteractorGLFW.h"
#include "Renderer.h"
#include "WindowGLFW.h"
#include <iostream>
#include <memory>

int main()
{
    auto renderer1 = std::make_shared<Jelly::Renderer>();
    renderer1->SetBackground({.2f, .2f, .2f, 1.f});
    renderer1->SetViewport({.05, .05, .4, .4});

    auto renderer2 = std::make_shared<Jelly::Renderer>();
    renderer2->SetBackground({.3f, .3f, .3f, 1.f});
    renderer2->SetViewport({.55, .55, .4, .4});

    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->AddRenderer(renderer1);
    window->AddRenderer(renderer2);
    window->SetSize(800, 600);
    window->SetTitle("Test window");
    window->Render();

    auto interactor = std::make_shared<Jelly::InteractorGLFW>();
    interactor->SetWindow(window);
    interactor->Start();
}
