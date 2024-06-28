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
#include "ShaderGenerator.h"
#include <memory>

namespace Jelly {
class DataSet;

class JELLY_EXPORT DataSetMapper : public Mapper
{
public:
    void Render(const vk::raii::CommandBuffer& commandBuffer, const vk::raii::RenderPass& renderPass) noexcept override;

    void SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept;

private:
    std::shared_ptr<DataSet> m_dataSet {};
    std::unique_ptr<ShaderGenerator> m_shaderGenerator {std::make_unique<ShaderGenerator>()};
};
} // namespace Jelly
