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
#include "Object.h"
#include <memory>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;
class DataSet;
enum class ColorMode : uint8_t;

class JELLY_EXPORT Drawable : public Object
{
    struct PrivateBufferData
    {
        bool initialized {false};
        std::unique_ptr<BufferData> bufferData {};
    };

public:
    std::vector<vk::Buffer> GetVertexBuffers() const noexcept;
    std::vector<vk::DeviceSize> GetVertexOffsets() const noexcept;

    /// @brief 获取顶点位置 Buffer
    /// @return
    vk::Buffer GetVertexPosBuffer() const noexcept;

    vk::Buffer GetIndexBuffer() const noexcept;
    uint32_t GetIndexCount() const noexcept;
    vk::IndexType GetIndexType() const noexcept;

    void SetDevice(std::shared_ptr<Device> device) noexcept;
    void SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept;
    void SetColorMode(ColorMode colorMode) noexcept;

    void Update() override;

private:
    std::shared_ptr<Device> m_device {};
    std::shared_ptr<DataSet> m_dataSet {};
    ColorMode m_colorMode {};

    std::unique_ptr<PrivateBufferData> m_vertexBufferData {std::make_unique<PrivateBufferData>()};
    std::unique_ptr<PrivateBufferData> m_colorBufferData {std::make_unique<PrivateBufferData>()};
    std::unique_ptr<PrivateBufferData> m_texCoordBufferData {std::make_unique<PrivateBufferData>()};
    std::unique_ptr<PrivateBufferData> m_indexBufferData {std::make_unique<PrivateBufferData>()};

    uint32_t m_indexCount {};
    vk::IndexType m_indexType {};
};
} // namespace Jelly
