#include "DataSet.h"

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
    return m_points != nullptr;
}

bool DataSet::HasColorData() const noexcept
{
    return m_colors != nullptr;
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
