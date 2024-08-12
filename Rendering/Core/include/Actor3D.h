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
    ~Actor3D() noexcept override
    {
        std::cout << __func__ << std::endl;
    }
    
    void Render(const vk::raii::CommandBuffer& commandBuffer, Renderer* renderer) noexcept override;
};
} // namespace Jelly
