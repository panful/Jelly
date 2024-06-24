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

#include "Object.h"
#include <atomic>
#include <memory>

namespace Jelly {
class Device;

class JELLY_EXPORT Mapper : public Object
{
public:
    virtual void Render() noexcept = 0;

    void SetDevice(std::shared_ptr<Device> device) noexcept;

protected:
    std::shared_ptr<Device> m_device {};

    std::atomic_bool m_needUpdate {true};
};
} // namespace Jelly
