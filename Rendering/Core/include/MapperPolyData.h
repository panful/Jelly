/**
 * @file MapperPolyData.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Mapper.h"

namespace Jelly {
class JELLY_EXPORT MapperPolyData : public Mapper
{
public:
    void Render() noexcept override;
};
} // namespace Jelly
