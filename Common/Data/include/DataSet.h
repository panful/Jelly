/**
 * @file DataSet.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-25
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#pragma warning(disable : 4251)

#include "Object.h"
#include <vector>

namespace Jelly {
struct Point
{
    float x {};
    float y {};
    float z {};

    static constexpr uint32_t GetSize() noexcept
    {
        return sizeof(Point);
    }
};

struct Color
{
    float r {};
    float g {};
    float b {};

    static constexpr uint32_t GetSize() noexcept
    {
        return sizeof(Color);
    }
};

struct TexCoord
{
    float u {};
    float v {};

    static constexpr uint32_t GetSize() noexcept
    {
        return sizeof(TexCoord);
    }
};

enum class PrimitiveType : uint8_t
{
    Vertex,
    Line,
    Triangle,
};

class JELLY_EXPORT DataSet : public Object
{
public:
    void SetPoints(std::vector<Point>&& points) noexcept;

    void SetColors(std::vector<Color>&& colors) noexcept;

    void SetIndices(std::vector<uint32_t>&& indices) noexcept;

    void SetPrimitiveType(PrimitiveType primitiveType) noexcept;

    constexpr PrimitiveType GetPrimitiveType() const noexcept;

    /// @brief 获取索引数据的大小
    /// @return
    constexpr size_t GetIndexSize() const noexcept;
    constexpr size_t GetPointSize() const noexcept;
    constexpr size_t GetColorSize() const noexcept;

    // /// @brief 获取每个索引的大小
    // /// @return
    // constexpr size_t GetIndexElementSize() const noexcept;
    // constexpr size_t GetPointElementSize() const noexcept;
    // constexpr size_t GetColorElementSize() const noexcept;

    /// @brief 获取索引的个数
    /// @return
    constexpr size_t GetIndexCount() const noexcept;
    constexpr size_t GetPointCount() const noexcept;

    constexpr bool HasColorData() const noexcept;

    /// @brief 每个顶点属性的字节大小
    /// @return
    std::vector<uint32_t> GetStrides() const noexcept;

    const std::vector<Point>& GetPoints() const noexcept;
    const std::vector<Color>& GetColors() const noexcept;
    const std::vector<uint32_t>& GetIndices() const noexcept;

private:
    std::vector<Point> m_points {};
    std::vector<Color> m_colors {};
    std::vector<uint32_t> m_indices {};

    PrimitiveType m_primitiveType {PrimitiveType::Triangle};
};
} // namespace Jelly
