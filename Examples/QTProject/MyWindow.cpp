#include "MyWindow.h"
#include "Actor3D.h"
#include "Camera.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "FloatData.h"
#include "InteractorQT.h"
#include "InteractorStyleTrackballCamera.h"
#include "Picker.h"
#include "Renderer.h"
#include "Texture.h"
#include "WindowQT.h"
#include <QBoxLayout>
#include <QButtonGroup>
#include <QCheckBox>
#include <QColorDialog>
#include <QDialog>
#include <QDockWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QWidget>
#include <functional>
#include <memory>
#include <vector>

class MyStyle : public Jelly::InteractorStyleTrackballCamera
{
protected:
    void MiddleButtonPressEvent() override
    {
        FindPokedRenderer();

        if (auto interactor = m_interactor.lock())
        {
            if (auto renderer = m_currentRenderer.lock())
            {
                auto pickedActor = m_picker->Pick(interactor->GetMousePosition(), renderer);
                if (m_pickCallback)
                {
                    m_pickCallback(pickedActor);
                }
            }
        }
    }

    void MiddleButtonReleaseEvent() override
    {
    }

public:
    void SetPickCallback(std::function<void(const std::shared_ptr<Jelly::Actor>&)>&& callback)
    {
        m_pickCallback = std::move(callback);
    }

private:
    std::unique_ptr<Jelly::Picker> m_picker {std::make_unique<Jelly::Picker>()};
    std::function<void(const std::shared_ptr<Jelly::Actor>&)> m_pickCallback {};
};

class ActorDialog : public QDialog
{
public:
    explicit ActorDialog(
        std::shared_ptr<Jelly::WindowQT>& window, std::unique_ptr<Actor>& actor, QPlainTextEdit* log, const char* name
    )
        : QDialog(nullptr)
    {
        this->setWindowTitle(name);
        this->setMinimumSize(400, 300);

        auto mainLayout = new QVBoxLayout(this);
        this->setLayout(mainLayout);

        //--------------------------------------
        auto colorTypeBox = new QGroupBox("color type", this);

        auto color_vertex  = new QRadioButton("Vertex", colorTypeBox);
        auto color_uniform = new QRadioButton("Uniform", colorTypeBox);
        auto color_texture = new QRadioButton("Texture", colorTypeBox);

        auto colorTypeBoxLayout = new QHBoxLayout(colorTypeBox);
        colorTypeBoxLayout->addWidget(color_vertex);
        colorTypeBoxLayout->addWidget(color_uniform);
        colorTypeBoxLayout->addWidget(color_texture);

        colorTypeBox->setLayout(colorTypeBoxLayout);

        auto colorTypeGroup = new QButtonGroup(colorTypeBox);
        colorTypeGroup->addButton(color_vertex, static_cast<int>(Jelly::ColorMode::Vertex));
        colorTypeGroup->addButton(color_uniform, static_cast<int>(Jelly::ColorMode::Uniform));
        colorTypeGroup->addButton(color_texture, static_cast<int>(Jelly::ColorMode::Texture));

        qobject_cast<QRadioButton*>(colorTypeGroup->button(static_cast<int>(actor->mapper->GetColorMode())))
            ->setChecked(true);

        connect(colorTypeGroup, &QButtonGroup::idClicked, [&actor, &window, &name, log](int id) {
            actor->mapper->SetColorMode(static_cast<Jelly::ColorMode>(id));
            log->appendPlainText(QString(" color mode: %1").arg(id));
            window->Render();
        });

        //----------------------------------------------
        auto visibilityBox       = new QGroupBox("visibility", this);
        auto visibilityBoxLayout = new QHBoxLayout(visibilityBox);
        visibilityBox->setLayout(visibilityBoxLayout);
        auto visible = new QCheckBox("visible", visibilityBox);
        visible->setChecked(actor->actor->GetVisibility());
        visibilityBoxLayout->addWidget(visible);

        connect(visible, &QCheckBox::stateChanged, [&window, &actor, &name, log]() {
            actor->actor->SetVisibility(!actor->actor->GetVisibility());
            log->appendPlainText(QString(" visible: %1").arg(actor->actor->GetVisibility() ? "on" : "off"));
            window->Render();
        });

        //----------------------------------------------
        auto lightingBox       = new QGroupBox("lighting", this);
        auto lightingBoxLayout = new QHBoxLayout(lightingBox);
        lightingBox->setLayout(lightingBoxLayout);
        auto lighting = new QCheckBox("lighting", lightingBox);
        lighting->setChecked(actor->actor->GetEnableLighting());
        lightingBoxLayout->addWidget(lighting);

        connect(lighting, &QCheckBox::stateChanged, [&window, &actor, &name, log]() {
            actor->actor->SetEnableLighting(!actor->actor->GetEnableLighting());
            log->appendPlainText(QString(" lighting: %1").arg(actor->actor->GetEnableLighting() ? "on" : "off"));
            window->Render();
        });

        //----------------------------------------------
        auto colorBox       = new QGroupBox("color", this);
        auto colorBoxLayout = new QHBoxLayout(colorBox);
        colorBox->setLayout(colorBoxLayout);
        auto color = new QPushButton("select color", colorBox);
        colorBoxLayout->addWidget(color);

        connect(color, &QPushButton::clicked, [&window, &actor, &name, log, this]() {
            QColorDialog dialog;
            dialog.setGeometry(this->geometry());
            dialog.exec();

            QColor c = dialog.selectedColor();
            log->appendPlainText(QString(" R: %1 G: %2 B: %3").arg(c.redF()).arg(c.greenF()).arg(c.blueF()));
            actor->actor->SetColor({c.redF(), c.greenF(), c.blueF()});
            window->Render();
        });

        //-------------------------------------------------
        mainLayout->addWidget(colorTypeBox);
        mainLayout->addWidget(visibilityBox);
        mainLayout->addWidget(lightingBox);
        mainLayout->addWidget(colorBox);
    }
};

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
    auto btnProject    = new QPushButton("camera project", widget);

    connect(btnRestCamera, &QPushButton::clicked, [this]() {
        this->m_defaultRenderer->ResetCamera();
        this->m_renderWindow->Render();
    });
    connect(btnBackground, &QPushButton::clicked, [this]() {
        static bool enable {false};
        enable = !enable;
        this->m_defaultRenderer->SetEnableGradientBackground(enable);
        this->m_renderWindow->Render();
    });
    connect(btnProject, &QPushButton::clicked, [this]() {
        static int index {1};
        this->m_defaultRenderer->GetCamera()->SetCameraType(static_cast<Jelly::CameraType>(index++ % 2));
        m_renderWindow->Render();
    });

    auto btnActor1 = new QPushButton("Actor 1", widget);
    auto btnActor2 = new QPushButton("Actor 2", widget);
    auto btnActor3 = new QPushButton("Actor 3", widget);

    connect(btnActor1, &QPushButton::clicked, [this]() {
        ActorDialog dialog(this->m_renderWindow, this->m_plane1, this->m_logTextWidget, "Actor 1");
        dialog.setGeometry(this->geometry().x(), this->geometry().y(), 100, 100);
        dialog.exec();
    });
    connect(btnActor2, &QPushButton::clicked, [this]() {
        ActorDialog dialog(this->m_renderWindow, this->m_plane2, this->m_logTextWidget, "Actor 2");
        dialog.setGeometry(this->geometry().x(), this->geometry().y(), 100, 100);
        dialog.exec();
    });
    connect(btnActor3, &QPushButton::clicked, [this]() {
        ActorDialog dialog(this->m_renderWindow, this->m_plane3, this->m_logTextWidget, "Actor 3");
        dialog.setGeometry(this->geometry().x(), this->geometry().y(), 100, 100);
        dialog.exec();
    });

    layout->addWidget(btnRestCamera);
    layout->addWidget(btnBackground);
    layout->addWidget(btnProject);
    layout->addWidget(btnActor1);
    layout->addWidget(btnActor2);
    layout->addWidget(btnActor3);
    widget->setLayout(layout);
    leftDockWidget->setWidget(widget);

    m_logTextWidget = new QPlainTextEdit(bottomDockWidget);
    m_logTextWidget->setReadOnly(true);
    bottomDockWidget->setWidget(m_logTextWidget);

    m_defaultRenderer = std::make_shared<Jelly::Renderer>();
    m_defaultRenderer->SetBackground({.1f, .2f, .3f, 1.f});
    m_renderWindow->AddRenderer(m_defaultRenderer);

    auto style = std::make_shared<MyStyle>();
    style->SetPickCallback([this](const std::shared_ptr<Jelly::Actor>& actor) {
        if (actor == this->m_plane1->actor)
        {
            this->m_logTextWidget->appendPlainText("picked actor1");
        }
        else if (actor == this->m_plane2->actor)
        {
            this->m_logTextWidget->appendPlainText("picked actor2");
        }
        else if (actor == this->m_plane3->actor)
        {
            this->m_logTextWidget->appendPlainText("picked actor3");
        }
    });

    m_windowInteractor = std::make_shared<Jelly::InteractorQT>();
    m_windowInteractor->SetWindow(m_renderWindow);
    m_windowInteractor->SetInteractorStyle(style);
    m_windowInteractor->Start();

    m_renderWindow->setFlags(Qt::FramelessWindowHint); // 必须设置为无边框，否则窗口会出现一个黑边
    QWidget* wrapper = QWidget::createWindowContainer(m_renderWindow.get());
    this->setCentralWidget(wrapper);

    InitActor();
}

void MyWindow::InitActor()
{
    auto makeCube = [](float x, float y, float z, float r, float g, float b) {
        // clang-format off
        std::vector<float> points {
            -1.f + x, -1.f + y, 0.f + z,
            -1.f + x,  1.f + y, 0.f + z,
             1.f + x,  1.f + y, 0.f + z,
             1.f + x, -1.f + y, 0.f + z
        };
        // clang-format on
        std::vector<float> colors {r, 0.f, 0.f, r, g, 0.f, 0.f, g, b, 0.f, 0.f, b};
        std::vector<float> texCoords {0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f};
        std::vector<uint32_t> cells {0, 1, 2, 0, 2, 3};

        auto pointData = std::make_shared<Jelly::FloatData>();
        pointData->SetData(std::move(points));

        auto colorData = std::make_shared<Jelly::FloatData>();
        colorData->SetData(std::move(colors));

        auto texCoordData = std::make_shared<Jelly::FloatData>();
        texCoordData->SetData(std::move(texCoords));

        auto indexData = std::make_shared<Jelly::DataArrayTemplate<uint32_t>>();
        indexData->SetData(std::move(cells));

        auto dataSet = std::make_shared<Jelly::DataSet>();
        dataSet->SetPoints(pointData);
        dataSet->SetColors(colorData);
        dataSet->SetTexCoords(texCoordData);
        dataSet->SetIndices(indexData);
        return dataSet;
    };

    auto makeTexture = [](uint8_t color) {
        std::vector<uint8_t> texturePixels {};
        texturePixels.reserve(100 * 100 * 4);
        for (auto x = 0u; x < 100u; ++x)
        {
            for (auto y = 0u; y < 100u; ++y)
            {
                texturePixels.emplace_back(x > 50 ? 0 : color);
                texturePixels.emplace_back(y > 50 ? 0 : color);
                texturePixels.emplace_back(color);
                texturePixels.emplace_back(255);
            }
        }

        auto texture = std::make_shared<Jelly::Texture>();
        texture->SetImage(vk::Extent2D {100, 100}, std::move(texturePixels));
        return texture;
    };

    //-----------------------------------
    m_plane1         = std::make_unique<Actor>();
    m_plane1->mapper = std::make_shared<Jelly::DataSetMapper>();
    m_plane1->mapper->SetDataSet(makeCube(0.f, 0.f, 0.f, 1.f, 1.f, 1.f));
    m_plane1->actor = std::make_shared<Jelly::Actor3D>();
    m_plane1->actor->SetMapper(m_plane1->mapper);
    m_plane1->actor->SetColor({1.f, 0.f, 0.f});
    m_plane1->actor->SetTexture(makeTexture(128));

    m_defaultRenderer->AddActor(m_plane1->actor);

    //-----------------------------------
    m_plane2         = std::make_unique<Actor>();
    m_plane2->mapper = std::make_shared<Jelly::DataSetMapper>();
    m_plane2->mapper->SetDataSet(makeCube(0.f, 1.f, 1.f, 1.f, 0.f, 1.f));
    m_plane2->actor = std::make_shared<Jelly::Actor3D>();
    m_plane2->actor->SetMapper(m_plane2->mapper);
    m_plane2->actor->SetColor({0.f, 1.f, 0.f});
    m_plane2->actor->SetTexture(makeTexture(64));

    m_defaultRenderer->AddActor(m_plane2->actor);

    //-----------------------------------
    m_plane3         = std::make_unique<Actor>();
    m_plane3->mapper = std::make_shared<Jelly::DataSetMapper>();
    m_plane3->mapper->SetDataSet(makeCube(0.f, -1.f, -1.f, 1.f, 1.f, 0.f));
    m_plane3->actor = std::make_shared<Jelly::Actor3D>();
    m_plane3->actor->SetMapper(m_plane3->mapper);
    m_plane3->actor->SetColor({0.f, 0.f, 1.f});
    m_plane3->actor->SetTexture(makeTexture(255));

    m_defaultRenderer->AddActor(m_plane3->actor);

    m_defaultRenderer->ResetCamera();
}
