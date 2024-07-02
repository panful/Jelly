/**
 * @file DataArrayTemplate.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-29
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "DataArray.h"
#include "Logger.h"
#include <algorithm>
#include <concepts>
#include <type_traits>
#include <vector>

namespace Jelly {
template <typename T>
concept AllowedDataType = std::same_as<T, char> || std::same_as<T, unsigned char> || std::same_as<T, short>
    || std::same_as<T, unsigned short> || std::same_as<T, int> || std::same_as<T, unsigned int> || std::same_as<T, long>
    || std::same_as<T, unsigned long> || std::same_as<T, long long> || std::same_as<T, unsigned long long>
    || std::same_as<T, float> || std::same_as<T, double>;

template <typename T>
    requires AllowedDataType<T>
struct DataTypeTraits;

template <>
struct DataTypeTraits<char>
{
    static constexpr DataType type {DataType::Char};
};

template <>
struct DataTypeTraits<unsigned char>
{
    static constexpr DataType type {DataType::UnsignedChar};
};

template <>
struct DataTypeTraits<short>
{
    static constexpr DataType type {DataType::Short};
};

template <>
struct DataTypeTraits<unsigned short>
{
    static constexpr DataType type {DataType::UnsignedShort};
};

template <>
struct DataTypeTraits<int>
{
    static constexpr DataType type {DataType::Int};
};

template <>
struct DataTypeTraits<unsigned int>
{
    static constexpr DataType type {DataType::UnsignedInt};
};

template <>
struct DataTypeTraits<long>
{
    static constexpr DataType type {DataType::Long};
};

template <>
struct DataTypeTraits<unsigned long>
{
    static constexpr DataType type {DataType::UnsignedLong};
};

template <>
struct DataTypeTraits<long long>
{
    static constexpr DataType type {DataType::LongLong};
};

template <>
struct DataTypeTraits<unsigned long long>
{
    static constexpr DataType type {DataType::UnsignedLongLong};
};

template <>
struct DataTypeTraits<float>
{
    static constexpr DataType type {DataType::Float};
};

template <>
struct DataTypeTraits<double>
{
    static constexpr DataType type {DataType::Double};
};

template <typename T>
class JELLY_EXPORT DataArrayTemplate : public DataArray
{
public:
    DataArrayTemplate()
    {
        SetDataType(DataTypeTraits<T>::type);
        SetDataTypeSize(sizeof(T));
    }

    void SetData(const std::vector<T>& data)
    {
        m_data = data;
    }

    void SetData(std::vector<T>&& data) noexcept
    {
        m_data = std::move(data);
    }

    void SetData(const T* data, uint32_t size)
    {
        m_data.assign(data, data + size);
    }

    template <typename OtherT>
        requires std::convertible_to<OtherT, T>
    void SetData(const std::vector<OtherT>& data) noexcept
    {
        m_data.resize(data.size());
        std::transform(data.cbegin(), data.cend(), m_data.begin(), [](auto val) { return static_cast<T>(val); });
    }

    const std::vector<T>& GetData() const noexcept
    {
        return m_data;
    }

    const T* GetPointer()
    {
        return m_data.data();
    }

public:
    bool Empty() override
    {
        return m_data.empty();
    }

    void Reserve(uint32_t size) override
    {
        m_data.reserve(size);
    }

    void Resize(uint32_t size) override
    {
        m_data.resize(size);
    }

    const void* GetVoidPointer() const noexcept override
    {
        return m_data.data();
    }

    uint32_t GetElementCount() const noexcept override
    {
        return static_cast<uint32_t>(m_data.size());
    }

private:
    std::vector<T> m_data {};
};
} // namespace Jelly
