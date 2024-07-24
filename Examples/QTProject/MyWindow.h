#include <QMainWindow>
#include <memory>

class QPlainTextEdit;

namespace Jelly {
class WindowQT;
class InteractorQT;
class Renderer;
class Actor;
class Mapper;
} // namespace Jelly

class MyWindow : public QMainWindow
{
    Q_OBJECT;

public:
    explicit MyWindow(QWidget* parent = nullptr);

private slots:
    void Background();
    void ResetCamera();
    void DisplayCube();
    void ProjectType();
    void ColorType();
    void Lighting();

private:
    std::shared_ptr<Jelly::WindowQT> m_renderWindow {};
    std::shared_ptr<Jelly::InteractorQT> m_windowInteractor {};
    std::shared_ptr<Jelly::Renderer> m_defaultRenderer {};
    std::shared_ptr<Jelly::Actor> m_cubeActor {};
    std::shared_ptr<Jelly::Mapper> m_cubeMapper {};

    QPlainTextEdit* m_logTextWidget {nullptr};
};
