#include "Pipeline.h"
#include "Device.h"
#include <array>

using namespace Jelly;

Pipeline::Pipeline(std::shared_ptr<Device> device, const PipelineInfo& pipelineInfo) noexcept
{
    vk::raii::ShaderModule vertexShaderModule(
        device->GetDevice(), vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), pipelineInfo.vertexShaderCode)
    );

    vk::raii::ShaderModule fragmentShaderModule(
        device->GetDevice(), vk::ShaderModuleCreateInfo(vk::ShaderModuleCreateFlags(), pipelineInfo.fragmentShaderCode)
    );

    std::array pipelineShaderStageCreateInfos = {
        vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eVertex, vertexShaderModule, "main", nullptr),
        vk::PipelineShaderStageCreateInfo({}, vk::ShaderStageFlagBits::eFragment, fragmentShaderModule, "main", nullptr)
    };

    //----------------------------------------
    uint32_t offset {0};
    uint32_t location {0};
    std::vector<vk::VertexInputAttributeDescription> inputAttributes {};
    for (auto& stride : pipelineInfo.strides)
    {
        inputAttributes.emplace_back(location, 0, vk::Format::eR32G32B32Sfloat, offset); // XXX
        offset += stride;
        location++;
    }
    std::array inputBindings {
        vk::VertexInputBindingDescription {0, offset, vk::VertexInputRate::eVertex}
    };

    vk::PipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo({}, inputBindings, inputAttributes);

    //----------------------------------------
    vk::PipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(
        vk::PipelineInputAssemblyStateCreateFlags(), pipelineInfo.topology
    );

    vk::PipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(
        vk::PipelineViewportStateCreateFlags(), 1, nullptr, 1, nullptr
    );

    vk::PipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(
        vk::PipelineRasterizationStateCreateFlags(),
        false,
        false,
        pipelineInfo.polygonMode,
        pipelineInfo.cullMode,
        pipelineInfo.frontFace,
        pipelineInfo.depthBiasEnable,
        0.f,
        0.f,
        0.f,
        1.f
    );

    vk::PipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo({}, pipelineInfo.sampleCount);

    vk::PipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(
        vk::PipelineDepthStencilStateCreateFlags(),
        pipelineInfo.depthTestEnable,
        pipelineInfo.depthWriteEnable,
        pipelineInfo.depthCompareOp,
        false,
        pipelineInfo.StencilTestEnable
    );

    //----------------------------------------
    vk::ColorComponentFlags colorComponentFlags(
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB
        | vk::ColorComponentFlagBits::eA
    );
    vk::PipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(
        pipelineInfo.blendEnable,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        vk::BlendFactor::eZero,
        vk::BlendFactor::eZero,
        vk::BlendOp::eAdd,
        colorComponentFlags
    );
    vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(
        vk::PipelineColorBlendStateCreateFlags(), false, vk::LogicOp::eNoOp, pipelineColorBlendAttachmentState
    );

    //----------------------------------------
    vk::PipelineDynamicStateCreateInfo pipelineDynamicStateCreateInfo(
        vk::PipelineDynamicStateCreateFlags(), pipelineInfo.dynamicStates
    );

    m_pipelineLayout = vk::raii::PipelineLayout(device->GetDevice(), vk::PipelineLayoutCreateInfo());

    vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo(
        vk::PipelineCreateFlags(),
        pipelineShaderStageCreateInfos,
        &pipelineVertexInputStateCreateInfo,
        &pipelineInputAssemblyStateCreateInfo,
        nullptr,
        &pipelineViewportStateCreateInfo,
        &pipelineRasterizationStateCreateInfo,
        &pipelineMultisampleStateCreateInfo,
        &pipelineDepthStencilStateCreateInfo,
        &pipelineColorBlendStateCreateInfo,
        &pipelineDynamicStateCreateInfo,
        m_pipelineLayout,
        pipelineInfo.renderPass
    );

    vk::raii::PipelineCache pipelineCache(device->GetDevice(), vk::PipelineCacheCreateInfo());
    m_pipeline = vk::raii::Pipeline(device->GetDevice(), pipelineCache, graphicsPipelineCreateInfo);
}

const vk::raii::PipelineLayout& Pipeline::GetPipelineLayout() const noexcept
{
    return m_pipelineLayout;
}

const vk::raii::Pipeline& Pipeline::GetPipeline() const noexcept
{
    return m_pipeline;
}
