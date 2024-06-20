#include "WindowGLFW.h"
#include <iostream>

int main()
{
    Jelly::WindowGLFW window {};
    window.SetSize(400, 300);
    window.SetTitle("test window");
    window.Render();
}
