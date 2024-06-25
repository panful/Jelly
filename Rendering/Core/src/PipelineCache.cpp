#include "PipelineCache.h"

using namespace Jelly;

bool PipelineCache::HasPipeline(size_t uid) const noexcept
{
    return m_pipelines.contains(uid);
}

void PipelineCache::AddPipeline(size_t uid, std::shared_ptr<Pipeline> pipeline) noexcept
{
    m_pipelines[uid] = std::move(pipeline);
}

void PipelineCache::AddPipeline(std::pair<size_t, std::shared_ptr<Pipeline>>&& pipeline) noexcept
{
    if (HasPipeline(pipeline.first))
    {
        m_pipelines.erase(pipeline.first);
    }

    m_pipelines.emplace(std::move(pipeline));
}

std::shared_ptr<Pipeline> PipelineCache::GetPipeline(size_t uid) const noexcept
{
    return HasPipeline(uid) ? m_pipelines.at(uid) : nullptr;
}
