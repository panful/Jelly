/**
 * @file RenderPass.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;

class JELLY_EXPORT RenderPass : public Object
{
public:
    RenderPass(std::shared_ptr<Device> device, vk::Format colorFormat, vk::Format depthFormat);

    const vk::raii::RenderPass& GetRenderPass() const noexcept;

private:
    std::shared_ptr<Device> m_device {};
    vk::raii::RenderPass m_renderPass {nullptr};
};
} // namespace Jelly
