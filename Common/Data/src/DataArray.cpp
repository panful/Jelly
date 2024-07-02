#include "DataArray.h"

using namespace Jelly;

void DataArray::SetDataType(DataType dataType) noexcept
{
    m_dataType = dataType;
}

DataType DataArray::GetDataType() const noexcept
{
    return m_dataType;
}

void DataArray::SetDataTypeSize(uint32_t dataTypeSize) noexcept
{
    m_dataTypeSize = dataTypeSize;
}

uint32_t DataArray::GetDataTypeSize() const noexcept
{
    return m_dataTypeSize;
}
