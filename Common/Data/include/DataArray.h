/**
 * @file DataArray.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <cstdint>
#include <vector>

namespace Jelly {
enum class DataType : uint8_t
{
    Void, // 无效的类型，正常情况下不应该是这种类型
    Char,
    UnsignedChar,
    Short,
    UnsignedShort,
    Int,
    UnsignedInt,
    Long,
    UnsignedLong,
    LongLong,
    UnsignedLongLong,
    Float,
    Double,
};

class JELLY_EXPORT DataArray : public Object
{
public:
    virtual bool Empty() = 0;

    virtual void Reserve(uint32_t size) = 0;

    virtual void Resize(uint32_t size) = 0;

    virtual const void* GetVoidPointer() const noexcept = 0;

    virtual uint32_t GetElementCount() const noexcept = 0;

    DataType GetDataType() const noexcept;

    uint32_t GetDataTypeSize() const noexcept;

protected:
    void SetDataType(DataType dataType) noexcept;

    void SetDataTypeSize(uint32_t dataTypeSize) noexcept;

private:
    DataType m_dataType {DataType::Void};
    uint32_t m_dataTypeSize {0};
};
} // namespace Jelly
