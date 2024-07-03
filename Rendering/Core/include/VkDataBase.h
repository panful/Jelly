/**
 * @file VkDataBase.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "GlobalExport.h"

namespace Jelly {
class JELLY_EXPORT VkDataBase
{
public:
    VkDataBase()                                 = default;
    VkDataBase(VkDataBase&&) noexcept            = default;
    VkDataBase& operator=(VkDataBase&&) noexcept = default;
    virtual ~VkDataBase() noexcept               = default;

private:
    VkDataBase(const VkDataBase&)            = delete;
    VkDataBase& operator=(const VkDataBase&) = delete;
};
} // namespace Jelly
