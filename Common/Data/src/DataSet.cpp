#include "DataSet.h"

using namespace Jelly;

void DataSet::SetPoints(std::vector<Point>&& points) noexcept
{
    m_points = std::move(points);
}

void DataSet::SetColors(std::vector<Color>&& colors) noexcept
{
    m_colors = std::move(colors);
}

void DataSet::SetIndices(std::vector<uint32_t>&& indices) noexcept
{
    m_indices = std::move(indices);
}

std::vector<uint32_t> DataSet::GetStrides() const noexcept
{
    std::vector<uint32_t> strides {Point::GetSize()};
    if (!m_colors.empty())
    {
        strides.emplace_back(Color::GetSize());
    }

    return strides;
}

void DataSet::SetPrimitiveType(PrimitiveType primitiveType) noexcept
{
    m_primitiveType = primitiveType;
}

constexpr PrimitiveType DataSet::GetPrimitiveType() const noexcept
{
    return m_primitiveType;
}

constexpr size_t DataSet::GetIndexSize() const noexcept
{
    return m_indices.size() * sizeof(uint32_t);
}

constexpr size_t DataSet::GetPointSize() const noexcept
{
    return m_points.size() * Point::GetSize();
}

constexpr size_t DataSet::GetColorSize() const noexcept
{
    return m_colors.size() * Color::GetSize();
}

// constexpr size_t DataSet::GetPointElementSize() const noexcept
// {
//     return Point::GetSize();
// }

// constexpr size_t DataSet::GetColorElementSize() const noexcept
// {
//     return Color::GetSize();
// }

// constexpr size_t DataSet::GetIndexElementSize() const noexcept
// {
//     return sizeof(uint32_t);
// }

constexpr size_t DataSet::GetIndexCount() const noexcept
{
    return m_indices.size();
}

constexpr bool DataSet::HasColorData() const noexcept
{
    return !m_colors.empty();
}

const std::vector<Point>& DataSet::GetPoints() const noexcept
{
    return m_points;
}

const std::vector<Color>& DataSet::GetColors() const noexcept
{
    return m_colors;
}

const std::vector<uint32_t>& DataSet::GetIndices() const noexcept
{
    return m_indices;
}
