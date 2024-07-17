/**
 * @file DataSetMapper.h
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
#include "ShaderCreater.h"
#include <memory>

namespace Jelly {
class DataSet;

class JELLY_EXPORT DataSetMapper : public Mapper
{
public:
    void Configure(const std::shared_ptr<Viewer>& viewer) noexcept override;

    std::array<double, 6> GetBounds() const noexcept override;

    void SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept;

    const std::shared_ptr<DataSet>& GetDataSet() const noexcept;

private:
    std::shared_ptr<DataSet> m_dataSet {};
    std::unique_ptr<ShaderCreater> m_shaderCreater {std::make_unique<ShaderCreater>()};
};
} // namespace Jelly
