/**
 * @file Actor.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <atomic>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Mapper;

class JELLY_EXPORT Actor : public Object
{
public:
    virtual void
    Render(const vk::raii::CommandBuffer& commandBuffer, const vk::raii::RenderPass& renderPass) noexcept = 0;

    void SetDevice(std::shared_ptr<Device> device) noexcept;
    void SetMapper(std::shared_ptr<Mapper> mapper) noexcept;

    void SetVisibility(bool visible) noexcept;
    bool GetVisibility() const noexcept;

protected:
    std::shared_ptr<Device> m_device {};
    std::shared_ptr<Mapper> m_mapper {};

    std::atomic_bool m_needUpdate {true};
    bool m_visibility {true};
};
} // namespace Jelly
