/**
 * @file PipelineCache.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Pipeline.h"
#include <memory>
#include <unordered_map>

namespace Jelly {
class PipelineCache
{
public:
    bool HasPipeline(size_t key) const noexcept;

    void AddPipeline(size_t key, std::unique_ptr<Pipeline>&& pipeline) noexcept;

    void AddPipeline(std::pair<size_t, std::unique_ptr<Pipeline>>&& pipeline) noexcept;

    const std::unique_ptr<Pipeline>& GetPipeline(size_t key) const;

private:
    std::unordered_map<size_t, std::unique_ptr<Pipeline>> m_pipelines {};
};
} // namespace Jelly
