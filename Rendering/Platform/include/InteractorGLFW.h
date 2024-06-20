/**
 * @file InteractorGLFW.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-20
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Interactor.h"

namespace Jelly {
class JELLY_EXPORT InteractorGLFW : public Interactor
{
public:
    void Start() const noexcept override;
};
} // namespace Jelly
