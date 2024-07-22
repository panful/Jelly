#include "WindowQT.h"
#include "Device.h"
#include "Logger.h"

using namespace Jelly;

WindowQT::WindowQT(QWindow* parent)
    : QWindow(parent)
{
}

WindowQT::WindowQT(std::shared_ptr<Device> device, QWindow* parent)
    : QWindow(parent)
    , Window(std::move(device))
{
}

void WindowQT::closeEvent(QCloseEvent*)
{
    // Qt会将 QVulkanInstance::surfaceForWindow 返回的 VkSurfaceKHR 销毁
    // SwapChain 必须保证在 Surface 销毁之前就销毁
    m_device->GetDevice().waitIdle();
    m_swapChainData.reset();
    m_surface.release();
}

void WindowQT::InitSurface() noexcept
{
    m_qVulkanInstance = std::make_unique<QVulkanInstance>();
    m_qVulkanInstance->setVkInstance(m_device->InitInstance());
    if (!m_qVulkanInstance->create())
    {
        Logger::GetInstance()->Error("failed to create QVulkanInstance");
    }

    this->setVulkanInstance(m_qVulkanInstance.get());
    this->setSurfaceType(QSurface::VulkanSurface);
    this->show();

    m_surface = vk::raii::SurfaceKHR(m_device->GetInstance(), QVulkanInstance::surfaceForWindow(this));
    m_window  = this;
}
