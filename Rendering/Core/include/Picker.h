/**
 * @file Picker.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include "PickRenderPass.h"
#include <array>
#include <memory>
#include <mutex>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Renderer;
class Actor;

class JELLY_EXPORT Picker : public Object
{
public:
    std::shared_ptr<Actor> Pick(const std::array<int, 2>& displayPos, std::shared_ptr<Renderer> renderer);

private:
    uint32_t GetActorID(const std::array<int, 2>& displayPos) const noexcept;

private:
    std::size_t m_pipelineKey {};
    std::once_flag m_createPipelineFlag {};
    std::unique_ptr<PickRenderPass> m_renderPass {};
    std::shared_ptr<Renderer> m_renderer {};
};
} // namespace Jelly
