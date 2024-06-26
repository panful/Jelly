#include "Drawable.h"
#include "BufferData.h"
#include "DataSet.h"
#include "Device.h"

using namespace Jelly;

Drawable::Drawable(std::shared_ptr<Device> device, std::shared_ptr<DataSet> dataSet)
    : m_indexBufferData(std::make_unique<BufferData>(
          device,
          dataSet->GetIndexSize(),
          vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
          vk::MemoryPropertyFlagBits::eDeviceLocal
      ))
    , m_indexCount(static_cast<uint32_t>(dataSet->GetIndexCount()))
{
    auto pointVertexBufferData = std::make_unique<BufferData>(
        device,
        dataSet->GetPointSize(),
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );
    pointVertexBufferData->Upload(device, dataSet->GetPoints());

    m_vertexBufferDatas.emplace_back(std::move(pointVertexBufferData));

    if (dataSet->HasColorData())
    {
        auto colorVertexBufferData = std::make_unique<BufferData>(
            device,
            dataSet->GetColorSize(),
            vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eDeviceLocal
        );
        colorVertexBufferData->Upload(device, dataSet->GetColors());

        m_vertexBufferDatas.emplace_back(std::move(colorVertexBufferData));
    }

    m_indexBufferData->Upload(device, dataSet->GetIndices());

    if (dataSet->GetIndexCount() > 0)
    {
        m_indexType = vk::IndexTypeValue<std::remove_cvref_t<decltype(dataSet->GetIndices().front())>>::value;
    }
}

std::vector<vk::Buffer> Drawable::GetVertexBuffers() const noexcept
{
    std::vector<vk::Buffer> buffers {};
    for (auto& vertexBufferData : m_vertexBufferDatas)
    {
        buffers.emplace_back(vertexBufferData->GetBuffer());
    }

    return buffers;
}

vk::Buffer Drawable::GetIndexBuffer() const noexcept
{
    return m_indexBufferData->GetBuffer();
}

uint32_t Drawable::GetIndexCount() const noexcept
{
    return m_indexCount;
}

vk::IndexType Drawable::GetIndexType() const noexcept
{
    return m_indexType;
}
