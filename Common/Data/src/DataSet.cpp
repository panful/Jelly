#include "DataSet.h"
#include "DataArray.h"
#include <algorithm>
#include <limits>

using namespace Jelly;

void DataSet::SetPoints(std::shared_ptr<DataArray> points) noexcept
{
    m_points = std::move(points);
}

void DataSet::SetColors(std::shared_ptr<DataArray> colors) noexcept
{
    m_colors = std::move(colors);
}

void DataSet::SetIndices(std::shared_ptr<DataArray> indices) noexcept
{
    m_indices = std::move(indices);
}

void DataSet::SetPrimitiveType(PrimitiveType primitiveType) noexcept
{
    m_primitiveType = primitiveType;
}

PrimitiveType DataSet::GetPrimitiveType() const noexcept
{
    return m_primitiveType;
}

bool DataSet::HasPointData() const noexcept
{
    return m_points != nullptr && !m_points->Empty();
}

bool DataSet::HasColorData() const noexcept
{
    return m_colors != nullptr && !m_colors->Empty();
}

bool DataSet::HasIndexData() const noexcept
{
    return m_indices != nullptr && !m_indices->Empty();
}

std::shared_ptr<DataArray> DataSet::GetPoints() const noexcept
{
    return m_points;
}

std::shared_ptr<DataArray> DataSet::GetColors() const noexcept
{
    return m_colors;
}

std::shared_ptr<DataArray> DataSet::GetIndices() const noexcept
{
    return m_indices;
}

std::array<double, 6> DataSet::GetBounds() noexcept
{
    static constexpr uint32_t pointComponents {3};
    if (m_points && m_points->GetElementCount() % pointComponents == 0 && IsChanged())
    {
        // XXX 其他类型需要补充
        if (m_points->GetDataType() == DataType::Float)
        {
            const float* data = static_cast<const float*>(m_points->GetVoidPointer());

            m_bounds[0] = std::numeric_limits<double>::max();
            m_bounds[1] = std::numeric_limits<double>::lowest();
            m_bounds[2] = std::numeric_limits<double>::max();
            m_bounds[3] = std::numeric_limits<double>::lowest();
            m_bounds[4] = std::numeric_limits<double>::max();
            m_bounds[5] = std::numeric_limits<double>::lowest();

            for (uint32_t i = 0; i + 2 < m_points->GetElementCount(); i += 3)
            {
                m_bounds[0] = std::min(static_cast<double>(data[i + 0]), m_bounds[0]);
                m_bounds[1] = std::max(static_cast<double>(data[i + 0]), m_bounds[1]);
                m_bounds[2] = std::min(static_cast<double>(data[i + 1]), m_bounds[2]);
                m_bounds[3] = std::max(static_cast<double>(data[i + 1]), m_bounds[3]);
                m_bounds[4] = std::min(static_cast<double>(data[i + 2]), m_bounds[4]);
                m_bounds[5] = std::max(static_cast<double>(data[i + 2]), m_bounds[5]);
            }
        }

        ResetChanged();
    }

    return m_bounds;
}
