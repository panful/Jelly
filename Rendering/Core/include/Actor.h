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
class Mapper;
class Viewer;
class Renderer;
class Texture;

class JELLY_EXPORT Actor : public Object
{
public:
    ~Actor() noexcept override
    {
        std::cout << __func__ << std::endl;
    }

    virtual void Render(const vk::raii::CommandBuffer& commandBuffer, Renderer* renderer) noexcept = 0;

    void SetMapper(std::shared_ptr<Mapper> mapper) noexcept;
    const std::shared_ptr<Mapper>& GetMapper() const noexcept;

    void SetVisibility(bool visible) noexcept;
    bool GetVisibility() const noexcept;

    void SetColor(const std::array<float, 3>& color);
    const std::array<float, 3>& GetColor() const noexcept;

    void SetTexture(std::shared_ptr<Texture> texture);
    const std::shared_ptr<Texture>& GetTexture() const noexcept;

    void SetEnableLighting(bool enableLighting) noexcept;
    bool GetEnableLighting() const noexcept;

    void SetModelMatrix(const std::array<float, 16>& modelMatrix);
    const std::array<float, 16> GetModelMatrix() const noexcept;

    std::array<double, 6> GetBounds() const noexcept;

protected:
    std::shared_ptr<Mapper> m_mapper {};

private:
    bool m_visibility {true};
    bool m_enableLighting {true};

    std::array<float, 16> m_modelMatrix {
        1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f
    };
    std::array<float, 3> m_color {1., 1., 1.};
    std::shared_ptr<Texture> m_texture {};
};
} // namespace Jelly
