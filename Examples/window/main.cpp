#include "InteractorGLFW.h"
#include "WindowGLFW.h"
#include <iostream>
#include <memory>

int main()
{
    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->SetSize(400, 300);
    window->SetTitle("Test window");
    window->Render();

    auto interactor = std::make_unique<Jelly::InteractorGLFW>();
    interactor->SetWindow(window);
    interactor->Start();
}
