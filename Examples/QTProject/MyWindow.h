#include <QMainWindow>
#include <memory>

namespace Jelly {
class WindowQT;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT;

public:
    explicit MyWindow(QWidget* parent = nullptr);

private:
    std::shared_ptr<Jelly::WindowQT> m_renderWindow {};
};
