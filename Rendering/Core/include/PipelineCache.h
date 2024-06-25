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

#include <memory>
#include <unordered_map>

namespace Jelly {
class Pipeline;

class PipelineCache
{
public:
    bool HasPipeline(size_t uid) const noexcept;

    void AddPipeline(size_t uid, std::shared_ptr<Pipeline> pipeline) noexcept;

    void AddPipeline(std::pair<size_t, std::shared_ptr<Pipeline>>&& pipeline) noexcept;

    std::shared_ptr<Pipeline> GetPipeline(size_t uid) const noexcept;

private:
    std::unordered_map<size_t, std::shared_ptr<Pipeline>> m_pipelines {};
};
} // namespace Jelly
