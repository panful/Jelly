/**
 * @file PickRenderPass.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "DepthImageData.h"
#include "RenderPass.h"

namespace Jelly {
class JELLY_EXPORT PickRenderPass : public RenderPass
{
public:
    PickRenderPass(const vk::Extent2D& extent);

    void Resize(const vk::Extent2D& extent) noexcept override;

    std::vector<vk::ClearValue> GetClearValues() const noexcept override;

    const std::unique_ptr<ImageData>& GetColorImageData() const noexcept;

private:
    vk::Format m_depthFormat {vk::Format::eD16Unorm};
    std::unique_ptr<DepthImageData> m_depthImageData {};
};
} // namespace Jelly
