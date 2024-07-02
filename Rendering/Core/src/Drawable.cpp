#include "Drawable.h"
#include "BufferData.h"
#include "DataArray.h"
#include "DataSet.h"
#include "Device.h"

using namespace Jelly;

Drawable::Drawable(std::shared_ptr<Device> device, std::shared_ptr<DataSet> dataSet)
    : m_indexBufferData(std::make_unique<BufferData>(
          device,
          dataSet->GetIndices()->GetDataTypeSize() * dataSet->GetIndices()->GetElementCount(),
          vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
          vk::MemoryPropertyFlagBits::eDeviceLocal
      ))
    , m_indexCount(dataSet->GetIndices()->GetElementCount())
{
    auto pointVertexBufferData = std::make_unique<BufferData>(
        device,
        dataSet->GetPoints()->GetElementCount() * sizeof(float),
        vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
        vk::MemoryPropertyFlagBits::eDeviceLocal
    );
    pointVertexBufferData->Upload(
        device,
        static_cast<const float*>(dataSet->GetPoints()->GetVoidPointer()),
        dataSet->GetPoints()->GetElementCount()
    );

    m_vertexBufferDatas.emplace_back(std::move(pointVertexBufferData));

    if (dataSet->HasColorData())
    {
        auto colorVertexBufferData = std::make_unique<BufferData>(
            device,
            dataSet->GetColors()->GetElementCount() * sizeof(float),
            vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
            vk::MemoryPropertyFlagBits::eDeviceLocal
        );
        colorVertexBufferData->Upload(
            device,
            static_cast<const float*>(dataSet->GetColors()->GetVoidPointer()),
            dataSet->GetColors()->GetElementCount()
        );

        m_vertexBufferDatas.emplace_back(std::move(colorVertexBufferData));
    }

    m_indexBufferData->Upload(
        device,
        static_cast<const uint32_t*>(dataSet->GetIndices()->GetVoidPointer()),
        dataSet->GetIndices()->GetElementCount()
    );

    m_indexType = vk::IndexType::eUint32;
    // if (dataSet->GetIndices()->GetElementCount() > 0)
    // {
    //    m_indexType = vk::IndexTypeValue<std::remove_cvref_t<decltype(dataSet->GetIndices().front())>>::value;
    // }
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

std::vector<vk::DeviceSize> Drawable::GetVertexOffsets() const noexcept
{
    std::vector<vk::DeviceSize> offsets {};
    offsets.resize(m_vertexBufferDatas.size(), 0);
    return offsets;
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
