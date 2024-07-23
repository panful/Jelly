#include "MyWindow.h"
#include "Renderer.h"
#include "WindowQT.h"
#include <QGridLayout>

MyWindow::MyWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_renderWindow(std::make_shared<Jelly::WindowQT>())
{
    QWidget* wrapper = QWidget::createWindowContainer(m_renderWindow.get());

    this->setCentralWidget(wrapper);

    auto renderer = std::make_shared<Jelly::Renderer>();
    m_renderWindow->AddRenderer(renderer);
}
