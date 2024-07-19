/**
 * @file DefaultRenderPass.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "RenderPass.h"
#include "DepthImageData.h"

namespace Jelly {
class JELLY_EXPORT DefaultRenderPass : public RenderPass
{
public:
    DefaultRenderPass(std::shared_ptr<Device> device, const vk::Extent2D& extent);

    void Resize(const vk::Extent2D& extent) noexcept override;

private:
    vk::Format m_depthFormat {vk::Format::eD16Unorm};
    std::unique_ptr<DepthImageData> m_depthImageData {nullptr};
};
} // namespace Jelly
