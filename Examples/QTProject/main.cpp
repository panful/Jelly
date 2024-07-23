#include "MyWindow.h"
#include <QApplication>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MyWindow window {};
    window.resize(800, 600);
    window.show();

    return app.exec();
}
