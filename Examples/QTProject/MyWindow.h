#include <QMainWindow>
#include <memory>

class QPlainTextEdit;

namespace Jelly {
class WindowQT;
class InteractorQT;
class Renderer;
class Actor;
class DataSetMapper;
} // namespace Jelly

struct Actor
{
    std::shared_ptr<Jelly::Actor> actor {};
    std::shared_ptr<Jelly::DataSetMapper> mapper {};
};

class MyWindow : public QMainWindow
{
    Q_OBJECT;

public:
    explicit MyWindow(QWidget* parent = nullptr);

private:
    void InitActor();

private:
    std::shared_ptr<Jelly::WindowQT> m_renderWindow {};
    std::shared_ptr<Jelly::InteractorQT> m_windowInteractor {};
    std::shared_ptr<Jelly::Renderer> m_defaultRenderer {};

    std::unique_ptr<Actor> m_plane1 {};
    std::unique_ptr<Actor> m_plane2 {};
    std::unique_ptr<Actor> m_plane3 {};

    QPlainTextEdit* m_logTextWidget {nullptr};
};
