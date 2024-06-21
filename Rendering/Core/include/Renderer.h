/**
 * @file Renderer.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"

namespace Jelly {
class JELLY_EXPORT Renderer : public Object
{
public:
    void Render() noexcept;
};
} // namespace Jelly
