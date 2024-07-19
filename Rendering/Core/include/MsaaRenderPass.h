/**
 * @file MSAARenderPass.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-19
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "DepthImageData.h"
#include "RenderPass.h"

namespace Jelly {
class JELLY_EXPORT MsaaRenderPass : public RenderPass
{
public:
    MsaaRenderPass(
        std::shared_ptr<Device> device, const vk::Extent2D& extent, vk::SampleCountFlagBits sampleCountFlagBits
    );

    void Resize(const vk::Extent2D& extent) noexcept override;

    std::vector<vk::ClearValue> GetClearValues() const noexcept override;

private:
    vk::Format m_depthFormat {vk::Format::eD16Unorm};
    std::unique_ptr<DepthImageData> m_depthImageData {};
    std::unique_ptr<ImageData> m_msaaColorImage {};
};
} // namespace Jelly
