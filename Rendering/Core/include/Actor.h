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
#include <array>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class Mapper;
class Viewer;
class Renderer;

class JELLY_EXPORT Actor : public Object
{
public:
    virtual void Render(
        const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
    ) noexcept = 0;

    void SetDevice(std::shared_ptr<Device> device) noexcept;

    void SetMapper(std::shared_ptr<Mapper> mapper) noexcept;
    const std::shared_ptr<Mapper>& GetMapper() const noexcept;

    void SetVisibility(bool visible) noexcept;
    bool GetVisibility() const noexcept;

    std::array<double, 6> GetBounds() const noexcept;

protected:
    std::shared_ptr<Device> m_device {};
    std::shared_ptr<Mapper> m_mapper {};

private:
    bool m_visibility {true};
};
} // namespace Jelly
