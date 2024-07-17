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

#include "Object.h"
#include <array>
#include <cstdint>
#include <memory>

namespace Jelly {
class DataArray;

enum class PrimitiveType : uint8_t
{
    Vertex,
    Line,
    Triangle,
};

class JELLY_EXPORT DataSet : public Object
{
public:
    void SetPoints(std::shared_ptr<DataArray> points) noexcept;

    void SetColors(std::shared_ptr<DataArray> colors) noexcept;

    void SetTexCoords(std::shared_ptr<DataArray> texCoords) noexcept;

    void SetIndices(std::shared_ptr<DataArray> indices) noexcept;

    void SetPrimitiveType(PrimitiveType primitiveType) noexcept;

    PrimitiveType GetPrimitiveType() const noexcept;

    bool HasPointData() const noexcept;
    bool HasColorData() const noexcept;
    bool HasTexCoordData() const noexcept;
    bool HasIndexData() const noexcept;

    const std::shared_ptr<DataArray>& GetPoints() const noexcept;
    const std::shared_ptr<DataArray>& GetColors() const noexcept;
    const std::shared_ptr<DataArray>& GetIndices() const noexcept;
    const std::shared_ptr<DataArray>& GetTexCoords() const noexcept;

    /// @brief 获取 AABB(Axis-Aligned Bounding Box)
    /// @return {xMin xMax yMin yMax zMin zMax}
    std::array<double, 6> GetBounds() noexcept;

private:
    std::shared_ptr<DataArray> m_points {};
    std::shared_ptr<DataArray> m_colors {};
    std::shared_ptr<DataArray> m_texCoords {};
    std::shared_ptr<DataArray> m_indices {};

    PrimitiveType m_primitiveType {PrimitiveType::Triangle};

    std::array<double, 6> m_bounds {-1., 1., -1., 1., -1., 1.};
};
} // namespace Jelly
