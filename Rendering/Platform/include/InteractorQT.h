/**
 * @file InteractorQT.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-22
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Interactor.h"

namespace Jelly {
class JELLY_EXPORT InteractorQT : public Interactor
{
public:
    void Start() noexcept override;
};
} // namespace Jelly
