/**
 * @file WindowQT.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Window.h"
#include <QVulkanInstance>
#include <QWindow>
#include <memory>

class QVulkanInstance;

namespace Jelly {
class JELLY_EXPORT WindowQT
    : public QWindow
    , public Window
{
public:
    explicit WindowQT(QWindow* parent = nullptr);

    explicit WindowQT(std::shared_ptr<Device> device, QWindow* parent = nullptr);

protected:
    void InitSurface() noexcept override;

    void closeEvent(QCloseEvent*) override;

private:
    std::unique_ptr<QVulkanInstance> m_qVulkanInstance {};
};
} // namespace Jelly
