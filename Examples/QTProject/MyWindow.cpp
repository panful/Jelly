#include "MyWindow.h"
#include "Actor3D.h"
#include "Camera.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "FloatData.h"
#include "InteractorQT.h"
#include "Renderer.h"
#include "Texture.h"
#include "WindowQT.h"
#include <QBoxLayout>
#include <QDockWidget>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <mutex>
#include <vector>

MyWindow::MyWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_renderWindow(std::make_shared<Jelly::WindowQT>())
{
    auto leftDockWidget = new QDockWidget(this);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftDockWidget);
    auto bottomDockWidget = new QDockWidget(this);
    this->addDockWidget(Qt::BottomDockWidgetArea, bottomDockWidget);

    auto widget = new QWidget(leftDockWidget);
    auto layout = new QVBoxLayout(widget);

    auto btnRestCamera = new QPushButton("reset camera", widget);
    auto btnBackground = new QPushButton("set background", widget);
    auto btnCube       = new QPushButton("display cube switch", widget);
    auto btnProject    = new QPushButton("camera project", widget);
    auto btnColor      = new QPushButton("coloring type", widget);
    auto btnLighting   = new QPushButton("light switch", widget);

    connect(btnRestCamera, &QPushButton::clicked, [this]() { this->ResetCamera(); });
    connect(btnBackground, &QPushButton::clicked, [this]() { this->Background(); });
    connect(btnCube, &QPushButton::clicked, [this]() { this->DisplayCube(); });
    connect(btnProject, &QPushButton::clicked, [this]() { this->ProjectType(); });
    connect(btnColor, &QPushButton::clicked, [this]() { this->ColorType(); });
    connect(btnLighting, &QPushButton::clicked, [this]() { this->Lighting(); });

    layout->addWidget(btnRestCamera);
    layout->addWidget(btnBackground);
    layout->addWidget(btnCube);
    layout->addWidget(btnProject);
    layout->addWidget(btnColor);
    layout->addWidget(btnLighting);
    widget->setLayout(layout);
    leftDockWidget->setWidget(widget);

    m_logTextWidget = new QPlainTextEdit(bottomDockWidget);
    m_logTextWidget->setReadOnly(true);
    bottomDockWidget->setWidget(m_logTextWidget);

    m_defaultRenderer = std::make_shared<Jelly::Renderer>();
    m_defaultRenderer->SetBackground({.1f, .2f, .3f, 1.f});
    m_renderWindow->AddRenderer(m_defaultRenderer);

    m_windowInteractor = std::make_shared<Jelly::InteractorQT>();
    m_windowInteractor->SetWindow(m_renderWindow);
    m_windowInteractor->Start();

    m_renderWindow->setFlags(Qt::FramelessWindowHint); // 必须设置为无边框，否则窗口会出现一个黑边
    QWidget* wrapper = QWidget::createWindowContainer(m_renderWindow.get());
    this->setCentralWidget(wrapper);
}

void MyWindow::Background()
{
    static bool enable {false};
    enable = !enable;
    m_defaultRenderer->SetEnableGradientBackground(enable);
    m_renderWindow->Render();
}

void MyWindow::ResetCamera()
{
    m_defaultRenderer->ResetCamera();
    m_renderWindow->Render();
}

void MyWindow::DisplayCube()
{
    static std::once_flag flag {};

    std::call_once(flag, [this]() {
        std::vector<float> points {
            -10.f, -10.f, -20.f, -10.f, 10.f, -20.f, 10.f, 10.f, -20.f, 10.f, -10.f, -20.f,
            -10.f, -10.f, 20.f,  -10.f, 10.f, 20.f,  10.f, 10.f, 20.f,  10.f, -10.f, 20.f,
        };

        std::vector<float> colors {
            1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
        };

        std::vector<float> texCoords {
            0.f,
            1.f,
            0.f,
            0.f,
            0.33f,
            1.f,
            0.33f,
            0.f,
            0.66f,
            1.f,
            0.66f,
            0.f,
            1.f,
            1.f,
            1.f,
            0.f,
        };

        std::vector<uint32_t> triangles {
            0, 1, 2, 0, 2, 3, 3, 2, 6, 3, 6, 7, 7, 6, 5, 7, 5, 4, 4, 5, 1, 4, 1, 0, 4, 0, 3, 4, 3, 7, 1, 5, 6, 1, 6, 2,
        };

        auto pointData = std::make_shared<Jelly::FloatData>();
        pointData->SetData(std::move(points));

        auto colorData = std::make_shared<Jelly::FloatData>();
        colorData->SetData(std::move(colors));

        auto texCoordData = std::make_shared<Jelly::FloatData>();
        texCoordData->SetData(std::move(texCoords));

        auto indexData = std::make_shared<Jelly::DataArrayTemplate<uint32_t>>();
        indexData->SetData(std::move(triangles));

        auto dataSet = std::make_shared<Jelly::DataSet>();
        dataSet->SetPoints(pointData);
        dataSet->SetColors(colorData);
        dataSet->SetTexCoords(texCoordData);
        dataSet->SetIndices(indexData);

        auto mapper = std::make_shared<Jelly::DataSetMapper>();
        mapper->SetDataSet(dataSet);
        mapper->SetColorMode(Jelly::ColorMode::Vertex);

        this->m_cubeMapper = mapper;

        std::vector<uint8_t> texturePixels {};
        texturePixels.reserve(100 * 100 * 4);
        for (auto x = 0u; x < 100u; ++x)
        {
            for (auto y = 0u; y < 100u; ++y)
            {
                texturePixels.emplace_back(x > 50 ? 0 : 255);
                texturePixels.emplace_back(y > 50 ? 0 : 255);
                texturePixels.emplace_back(0);
                texturePixels.emplace_back(255);
            }
        }

        auto texture = std::make_shared<Jelly::Texture>();
        texture->SetImage(vk::Extent2D {100, 100}, std::move(texturePixels));

        this->m_cubeActor = std::make_shared<Jelly::Actor3D>();
        this->m_cubeActor->SetMapper(this->m_cubeMapper);
        this->m_cubeActor->SetColor({1.f, 0.f, 1.f});
        this->m_cubeActor->SetTexture(texture);

        this->m_defaultRenderer->AddActor(this->m_cubeActor);
        this->m_defaultRenderer->ResetCamera();
    });

    static bool visible {false};
    visible = !visible;
    m_cubeActor->SetVisibility(visible);

    m_renderWindow->Render();
}

void MyWindow::ProjectType()
{
    static int index {1};
    m_defaultRenderer->GetCamera()->SetCameraType(static_cast<Jelly::CameraType>(index++ % 2));
    m_renderWindow->Render();
}

void MyWindow::ColorType()
{
    if (!m_cubeMapper)
    {
        return;
    }

    static int index {1};
    m_cubeMapper->SetColorMode(static_cast<Jelly::ColorMode>(index++ % 3));
    m_renderWindow->Render();
}

void MyWindow::Lighting()
{
    if (!m_cubeActor)
    {
        return;
    }

    m_cubeActor->SetEnableLighting(!m_cubeActor->GetEnableLighting());
    m_renderWindow->Render();

    m_logTextWidget->appendPlainText(QString("lighting: %1").arg(m_cubeActor->GetEnableLighting() ? "on" : "off"));
}
