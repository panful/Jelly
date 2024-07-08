/**
 * @file Actor3D.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Actor.h"

namespace Jelly {
class JELLY_EXPORT Actor3D : public Actor
{
public:
    void Render(
        const vk::raii::CommandBuffer& commandBuffer, const std::shared_ptr<Viewer>& viewer, Renderer* renderer
    ) noexcept override;
};
} // namespace Jelly
