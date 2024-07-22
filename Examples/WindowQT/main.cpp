#include "WindowQT.h"
#include <QGuiApplication>

int main(int argc, char** argv)
{
    QGuiApplication app(argc, argv);

    Jelly::WindowQT window {};
    window.resize(800, 600);
    window.Render();

    return app.exec();
}
