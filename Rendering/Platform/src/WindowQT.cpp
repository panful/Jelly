#include "WindowQT.h"
#include "Device.h"
#include "Logger.h"
#include <QResizeEvent>

using namespace Jelly;

WindowQT::WindowQT(QWindow* parent)
    : QWindow(parent)
{
}

void WindowQT::SetEventAdapter(std::function<void(QEvent*)>&& eventAdapter) noexcept
{
    m_eventAdapter = std::move(eventAdapter);
}

void WindowQT::InitSurface() noexcept
{
    m_qVulkanInstance = std::make_unique<QVulkanInstance>();
    m_qVulkanInstance->setVkInstance(Device::Get()->InitInstance());
    if (!m_qVulkanInstance->create())
    {
        Logger::GetInstance()->Error("failed to create QVulkanInstance");
    }

    this->setVulkanInstance(m_qVulkanInstance.get());
    this->setSurfaceType(QSurface::VulkanSurface);
    this->create();

    m_surface = vk::raii::SurfaceKHR(Device::Get()->GetInstance(), QVulkanInstance::surfaceForWindow(this));
    m_window  = this;
}

void WindowQT::resizeEvent(QResizeEvent* event)
{
    if (event->size().width() <= 0 || event->size().height() <= 0 || !m_swapChainData)
    {
        return QWindow::resizeEvent(event);
    }

    this->SetSize(static_cast<uint32_t>(event->size().width()), static_cast<uint32_t>(event->size().height()));
    this->Render();

    return QWindow::resizeEvent(event);
}

void WindowQT::paintEvent(QPaintEvent* event)
{
    this->Render();
    return QWindow::paintEvent(event);
}

bool WindowQT::event(QEvent* event)
{
    static bool destroySurface {false};
    if (event->type() == QEvent::PlatformSurface && !destroySurface)
    {
        destroySurface = true;
        return QWindow::event(event);
    }

    if (event->type() == QEvent::PlatformSurface && destroySurface)
    {
        // Qt 会将 QVulkanInstance::surfaceForWindow 返回的 VkSurfaceKHR 销毁
        // SwapChain 必须保证在 Surface 销毁之前就销毁
        Device::Get()->GetDevice().waitIdle();
        m_swapChainData.reset();
        m_surface.release();
        return true;
    }

    if (m_eventAdapter)
    {
        m_eventAdapter(event);
    }

    return QWindow::event(event);
}
