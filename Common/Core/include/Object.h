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
#include <atomic>

namespace Jelly {
class JELLY_EXPORT Object
{
public:
    Object() noexcept = default;

    virtual ~Object() noexcept = default;

    Object(const Object&) = delete;

    Object& operator=(const Object&) = delete;

    Object(Object&&) noexcept = default;

    Object& operator=(Object&&) noexcept = default;

    /// @brief 将当前状态设置为有修改
    void Changed() noexcept;

    /// @brief 获取当前状态
    /// @return true 表示有修改，false 表示未修改
    bool IsChanged() const noexcept;

    /// @brief 将状态设置为未修改
    void ResetChanged() noexcept;

private:
    std::atomic_bool m_isChanged {true};
};
} // namespace Jelly
