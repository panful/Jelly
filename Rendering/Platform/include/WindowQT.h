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
#include <functional>
#include <memory>

class QVulkanInstance;

namespace Jelly {
class JELLY_EXPORT WindowQT
    : public QWindow
    , public Window
{
public:
    explicit WindowQT(QWindow* parent = nullptr);

    void SetEventAdapter(std::function<void(QEvent*)>&& eventAdapter) noexcept;

protected:
    void InitSurface() noexcept override;

    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

    bool event(QEvent* event) override;

protected:
    std::function<void(QEvent*)> m_eventAdapter {};

private:
    std::unique_ptr<QVulkanInstance> m_qVulkanInstance {};
};
} // namespace Jelly
