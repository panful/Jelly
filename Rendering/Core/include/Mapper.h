/**
 * @file Mapper.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#pragma warning(disable : 4251)

#include "Drawable.h"
#include "Object.h"
#include "Pipeline.h"
#include <atomic>
#include <memory>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;

class JELLY_EXPORT Mapper : public Object
{
public:
    virtual void
    Render(const vk::raii::CommandBuffer& commandBuffer, const vk::raii::RenderPass& renderPass) noexcept = 0;

    void SetDevice(std::shared_ptr<Device> device) noexcept;

protected:
    void BuildPipeline(const vk::raii::RenderPass& renderPass, const PipelineInfo& pipelineInfo) noexcept;

protected:
    std::shared_ptr<Device> m_device {};
    std::unique_ptr<Drawable> m_drawable {};

    size_t m_pipelineKey {};

    std::atomic_bool m_needUpdate {true};
};
} // namespace Jelly
