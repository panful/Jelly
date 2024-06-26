#include "Pipeline.h"
#include "Device.h"
#include "Logger.h"
#include <array>
#include <format>

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
    auto getFormat = [](uint32_t stride) -> vk::Format {
        switch (stride / sizeof(float))
        {
            case 3:
                return vk::Format::eR32G32B32Sfloat;
            case 2:
                return vk::Format::eR32G32Sfloat;
            default:
                Logger::GetInstance()->Error(std::format("stride error, current stride is {}", stride));
        }
        return {};
    };

    uint32_t locationBinding {0};
    std::vector<vk::VertexInputAttributeDescription> inputAttributes {};
    std::vector<vk::VertexInputBindingDescription> inputBindings {};
    for (auto& stride : pipelineInfo.strides)
    {
        inputAttributes.emplace_back(locationBinding, locationBinding, getFormat(stride), 0);
        inputBindings.emplace_back(locationBinding, stride, vk::VertexInputRate::eVertex);
        locationBinding++;
    }

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
