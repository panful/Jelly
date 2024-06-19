/**
 * @file Object.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-18
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "GlobalExport.h"

namespace Jelly {
class JELLY_EXPORT Object
{
public:
    Object() noexcept                = default;
    virtual ~Object() noexcept       = default;
    Object(const Object&)            = delete;
    Object& operator=(const Object&) = delete;
};
} // namespace Jelly
