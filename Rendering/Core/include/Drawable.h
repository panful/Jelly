/**
 * @file Drawable.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "BufferData.h"
#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class DataSet;

class Drawable
{
public:
    Drawable(std::shared_ptr<Device> device, std::shared_ptr<DataSet> dataSet);

    Drawable(const Drawable&)            = delete;
    Drawable& operator=(const Drawable&) = delete;

    std::vector<vk::Buffer> GetVertexBuffers() const noexcept;
    vk::Buffer GetIndexBuffer() const noexcept;

    uint32_t GetIndexCount() const noexcept;
    vk::IndexType GetIndexType() const noexcept;

private:
    std::vector<std::unique_ptr<BufferData>> m_vertexBufferDatas {};
    std::unique_ptr<BufferData> m_indexBufferData {};

    uint32_t m_indexCount {};
    vk::IndexType m_indexType {};
};
} // namespace Jelly
