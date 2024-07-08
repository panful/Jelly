/**
 * @file Pipeline.h
 * @author yangpan (yangpan0822@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-06-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "Object.h"
#include <functional>
#include <string>
#include <vector>
#include <vulkan/vulkan_raii.hpp>

namespace Jelly {
class Device;

struct DescriptorSetLayoutBinding
{
    uint32_t binding {};
    vk::DescriptorType descriptorType {};
    vk::ShaderStageFlags stageFlags {};
};

struct PushConstantRange
{
    vk::ShaderStageFlags stageFlags {};
    uint32_t size {};
};

struct PipelineInfo
{
    std::vector<uint32_t> vertexShaderCode {};
    std::vector<uint32_t> fragmentShaderCode {};

    // 顶点着色器的输入数据（顶点数据布局）
    std::vector<uint32_t> strides {};

    // 拓扑类型，点、线、三角形
    vk::PrimitiveTopology topology {vk::PrimitiveTopology::eTriangleList};

    // 线框、填充
    vk::PolygonMode polygonMode {vk::PolygonMode::eFill};
    // 正面顶点顺序
    vk::FrontFace frontFace {vk::FrontFace::eCounterClockwise};
    // 表面剔除类型
    vk::CullModeFlagBits cullMode {vk::CullModeFlagBits::eNone};
    // 深度偏移
    bool depthBiasEnable {false};
    // 线宽
    float lineWidth {1.f};

    // 多重采样
    vk::SampleCountFlagBits sampleCount {vk::SampleCountFlagBits::e1};

    // 深度和模板测试
    bool depthTestEnable {true};
    bool depthWriteEnable {true};
    bool StencilTestEnable {false};
    vk::CompareOp depthCompareOp {vk::CompareOp::eLess};

    // 颜色混合
    bool blendEnable {false};

    // 动态状态
    std::vector<vk::DynamicState> dynamicStates {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

    std::vector<PushConstantRange> pushConstantRanges {};

    // 描述符集布局
    std::vector<DescriptorSetLayoutBinding> descriptorSetLayoutBindings {};

    vk::RenderPass renderPass {nullptr};
};

class JELLY_EXPORT Pipeline : public Object
{
public:
    Pipeline(std::shared_ptr<Device> device, const PipelineInfo& pipelineInfo) noexcept;

    const vk::raii::DescriptorSetLayout& GetDescriptorSetLayout() const noexcept;
    const vk::raii::PipelineLayout& GetPipelineLayout() const noexcept;
    const vk::raii::Pipeline& GetPipeline() const noexcept;

private:
    vk::raii::DescriptorSetLayout m_descriptorSetLayout {nullptr};
    vk::raii::PipelineLayout m_pipelineLayout {nullptr};
    vk::raii::Pipeline m_pipeline {nullptr};
};
} // namespace Jelly

namespace std {
template <>
struct hash<Jelly::PipelineInfo>
{
    size_t operator()(const Jelly::PipelineInfo& pipelineInfo) const noexcept
    {
        std::string infoStr {};
        infoStr.reserve(1024);
        infoStr += "#vertexShaderCode";
        infoStr.insert(infoStr.cend(), pipelineInfo.vertexShaderCode.begin(), pipelineInfo.vertexShaderCode.end());
        infoStr += "#fragmentShaderCode";
        infoStr.insert(infoStr.cend(), pipelineInfo.fragmentShaderCode.begin(), pipelineInfo.fragmentShaderCode.end());
        infoStr += "#strides";
        infoStr.insert(infoStr.cend(), pipelineInfo.strides.begin(), pipelineInfo.strides.end());
        infoStr += "#topology";
        infoStr += std::to_string(static_cast<int>(pipelineInfo.topology));
        infoStr += "#polygonMode";
        infoStr += std::to_string(static_cast<int>(pipelineInfo.polygonMode));
        infoStr += "#frontFace";
        infoStr += std::to_string(static_cast<int>(pipelineInfo.frontFace));
        infoStr += "#cullMode";
        infoStr += std::to_string(static_cast<int>(pipelineInfo.cullMode));
        infoStr += "#depthBiasEnable";
        infoStr += std::to_string(pipelineInfo.depthBiasEnable);
        infoStr += "#lineWidth";
        infoStr += std::to_string(pipelineInfo.lineWidth);
        infoStr += "#sampleCount";
        infoStr += std::to_string(static_cast<int>(pipelineInfo.sampleCount));
        infoStr += "#depthTestEnable";
        infoStr += std::to_string(pipelineInfo.depthTestEnable);
        infoStr += "#depthWriteEnable";
        infoStr += std::to_string(pipelineInfo.depthWriteEnable);
        infoStr += "#StencilTestEnable";
        infoStr += std::to_string(pipelineInfo.StencilTestEnable);
        infoStr += "#depthCompareOp";
        infoStr += std::to_string(static_cast<int>(pipelineInfo.depthCompareOp));
        infoStr += "#blendEnable";
        infoStr += std::to_string(pipelineInfo.blendEnable);
        infoStr += "#dynamicStates";
        for (auto& dynamicState : pipelineInfo.dynamicStates)
        {
            infoStr += std::to_string(static_cast<int>(dynamicState));
        }

        // TODO
        infoStr += "#descriptorSet";
        infoStr += "#renderPass";

        std::hash<std::string> hashPipelineInfo {};
        return hashPipelineInfo(infoStr);
    }
};
} // namespace std
