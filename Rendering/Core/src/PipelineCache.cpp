#include "PipelineCache.h"

using namespace Jelly;

bool PipelineCache::HasPipeline(size_t key) const noexcept
{
    return m_pipelines.contains(key);
}

void PipelineCache::AddPipeline(size_t key, std::unique_ptr<Pipeline>&& pipeline) noexcept
{
    m_pipelines[key] = std::move(pipeline);
}

void PipelineCache::AddPipeline(std::pair<size_t, std::unique_ptr<Pipeline>>&& pipeline) noexcept
{
    if (HasPipeline(pipeline.first))
    {
        m_pipelines.erase(pipeline.first);
    }

    m_pipelines.emplace(std::move(pipeline));
}

const std::unique_ptr<Pipeline>& PipelineCache::GetPipeline(size_t key) const
{
    return m_pipelines.at(key);
}
