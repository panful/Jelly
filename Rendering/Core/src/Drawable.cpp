#include "Drawable.h"
#include "BufferData.h"
#include "DataArray.h"
#include "DataSet.h"
#include "Device.h"
#include "Logger.h"
#include "Mapper.h"

using namespace Jelly;

void Drawable::SetDataSet(std::shared_ptr<DataSet> dataSet) noexcept
{
    if (m_dataSet != dataSet)
    {
        m_dataSet = std::move(dataSet);
        Changed();
    }
}

void Drawable::SetColorMode(ColorMode colorMode) noexcept
{
    if (m_colorMode != colorMode)
    {
        m_colorMode = colorMode;
        Changed();
    }
}

void Drawable::Update()
{
    if (IsChanged())
    {
        if (!m_indexBufferData->initialized)
        {
            m_indexBufferData->bufferData = std::make_unique<BufferData>(
                m_dataSet->GetIndices()->GetDataTypeSize() * m_dataSet->GetIndices()->GetElementCount(),
                vk::BufferUsageFlagBits::eIndexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            );

            m_indexBufferData->bufferData->Upload(
                static_cast<const uint32_t*>(m_dataSet->GetIndices()->GetVoidPointer()),
                m_dataSet->GetIndices()->GetElementCount()
            );

            if (DataType::UnsignedInt != m_dataSet->GetIndices()->GetDataType())
            {
                Logger::GetInstance()->Error("Currently, only uint32_t type is supported");
            }

            m_indexCount = m_dataSet->GetIndices()->GetElementCount();
            m_indexType  = vk::IndexType::eUint32;

            m_indexBufferData->initialized = true;
        }

        if (!m_vertexBufferData->initialized)
        {
            m_vertexBufferData->bufferData = std::make_unique<BufferData>(
                m_dataSet->GetPoints()->GetElementCount() * sizeof(float),
                vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            );

            m_vertexBufferData->bufferData->Upload(
                static_cast<const float*>(m_dataSet->GetPoints()->GetVoidPointer()),
                m_dataSet->GetPoints()->GetElementCount()
            );

            m_vertexBufferData->initialized = true;
        }

        if (!m_colorBufferData->initialized && ColorMode::Vertex == m_colorMode && m_dataSet->HasColorData())
        {
            m_colorBufferData->bufferData = std::make_unique<BufferData>(
                m_dataSet->GetColors()->GetElementCount() * sizeof(float),
                vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            );
            m_colorBufferData->bufferData->Upload(
                static_cast<const float*>(m_dataSet->GetColors()->GetVoidPointer()),
                m_dataSet->GetColors()->GetElementCount()
            );

            m_colorBufferData->initialized = true;
        }
        else if (ColorMode::Vertex == m_colorMode && !m_dataSet->HasColorData())
        {
            Logger::GetInstance()->Error("Color mode is Vertex, but not have color data");
        }

        if (!m_texCoordBufferData->initialized && ColorMode::Texture == m_colorMode && m_dataSet->HasTexCoordData())
        {
            m_texCoordBufferData->bufferData = std::make_unique<BufferData>(
                m_dataSet->GetTexCoords()->GetElementCount() * sizeof(float),
                vk::BufferUsageFlagBits::eVertexBuffer | vk::BufferUsageFlagBits::eTransferDst,
                vk::MemoryPropertyFlagBits::eDeviceLocal
            );
            m_texCoordBufferData->bufferData->Upload(
                static_cast<const float*>(m_dataSet->GetTexCoords()->GetVoidPointer()),
                m_dataSet->GetTexCoords()->GetElementCount()
            );

            m_texCoordBufferData->initialized = true;
        }
        else if (ColorMode::Texture == m_colorMode && !m_dataSet->HasTexCoordData())
        {
            Logger::GetInstance()->Error("Color mode is Texture, but not have color data");
        }

        ResetChanged();
    }
}

std::vector<vk::Buffer> Drawable::GetVertexBuffers() const noexcept
{
    std::vector<vk::Buffer> buffers {};
    buffers.emplace_back(m_vertexBufferData->bufferData->GetBuffer());

    switch (m_colorMode)
    {
        case ColorMode::Texture:
            buffers.emplace_back(m_texCoordBufferData->bufferData->GetBuffer());
            break;
        case ColorMode::Vertex:
            buffers.emplace_back(m_colorBufferData->bufferData->GetBuffer());
            break;
        default:
            break;
    }

    return buffers;
}

std::vector<vk::DeviceSize> Drawable::GetVertexOffsets() const noexcept
{
    std::vector<vk::DeviceSize> offsets {0};
    switch (m_colorMode)
    {
        case ColorMode::Texture:
            [[fallthrough]];
        case ColorMode::Vertex:
            offsets.emplace_back(0);
            break;
        default:
            break;
    }

    return offsets;
}

vk::Buffer Drawable::GetVertexPosBuffer() const noexcept
{
    return m_vertexBufferData->bufferData->GetBuffer();
}

vk::Buffer Drawable::GetIndexBuffer() const noexcept
{
    return m_indexBufferData->bufferData->GetBuffer();
}

uint32_t Drawable::GetIndexCount() const noexcept
{
    return m_indexCount;
}

vk::IndexType Drawable::GetIndexType() const noexcept
{
    return m_indexType;
}
