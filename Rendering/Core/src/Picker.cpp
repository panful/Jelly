#include "Picker.h"
#include "Actor.h"
#include "Device.h"
#include "Mapper.h"
#include "MemoryHelper.h"
#include "Pipeline.h"
#include "Renderer.h"
#include "SpvCreater.h"
#include "Viewer.h"
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace Jelly;

namespace {
std::string vertCode = R"(
#version 450

layout(location = 0) in vec3 inPos;

layout(push_constant) uniform PushConstant {
    mat4 mvp;
    vec3 id;
} pc;

void main()
{
    gl_Position = pc.mvp * vec4(inPos, 1.);
})";

std::string fragCode = R"(
#version 450

layout(push_constant) uniform PushConstant {
    mat4 mvp;
    vec3 id;
} pc;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(pc.id, 1.);
})";

auto Multi(const std::array<float, 16>& mat1, const std::array<float, 16>& mat2)
{
    std::array<float, 16> result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i * 4 + j] = 0;
            for (int k = 0; k < 4; ++k)
            {
                result[i * 4 + j] += mat1[i * 4 + k] * mat2[k * 4 + j];
            }
        }
    }

    return result;
};
} // namespace

std::shared_ptr<Actor> Picker::Pick(const std::array<int, 2>& displayPos, std::shared_ptr<Renderer> renderer)
{
    if (renderer->GetAllActors().empty())
    {
        return {};
    }

    auto extent = vk::Extent2D {
        static_cast<uint32_t>(renderer->GetViewer()->GetExtent().width * renderer->GetViewport()[2]),
        static_cast<uint32_t>(renderer->GetViewer()->GetExtent().height * renderer->GetViewport()[3])
    };

    if (m_renderer != renderer)
    {
        Changed();
        m_renderer = std::move(renderer);
    }
    else if (m_renderPass && m_renderPass->GetExtent() != extent)
    {
        Changed();
    }

    if (IsChanged())
    {
        if (m_renderPass)
        {
            m_renderPass->Resize(extent);
        }
        else
        {
            m_renderPass = std::make_unique<PickRenderPass>(extent);
        }

        // 所有的拾取都使用同一个着色器
        static auto vertSpv = SpvCreater::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eVertex, vertCode);
        static auto fragSpv = SpvCreater::GetInstance()->GLSL2SPV(vk::ShaderStageFlagBits::eFragment, fragCode);

        // 同一个 Picker 只需要创建一次 Pipeline
        std::call_once(m_createPipelineFlag, [this]() {
            // MVP(mat4) + PickID(vec3)
            std::vector<PushConstantRange> pushConstantRange {
                {vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, (3 + 16) * sizeof(float)}
            };

            PipelineInfo pipelineInfo {
                .vertexShaderCode   = vertSpv.value(),
                .fragmentShaderCode = fragSpv.value(),
                .strides            = {static_cast<uint32_t>(sizeof(float) * 3)}, // 只需要顶点位置 {x, y, z}
                .pushConstantRanges = std::move(pushConstantRange),
                .renderPass         = this->m_renderPass->GetRenderPass()
            };

            this->m_pipelineKey = std::hash<PipelineInfo>()(pipelineInfo);

            Device::Get()->GetPipelineCache()->AddPipeline(m_pipelineKey, std::make_unique<Pipeline>(pipelineInfo));
        });

        ResetChanged();
    }

    std::unordered_map<uint32_t, std::shared_ptr<Actor>> id_actors {};
    MemoryHelper::OneTimeSubmit([&id_actors, this](const vk::raii::CommandBuffer& cmd) {
        auto clearValues = this->m_renderPass->GetClearValues();
        vk::RenderPassBeginInfo renderPassBeginInfo(
            this->m_renderPass->GetRenderPass(),
            this->m_renderPass->GetFramebuffers().front(),
            vk::Rect2D({0, 0}, this->m_renderPass->GetExtent()),
            clearValues
        );
        cmd.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        auto& pipeline = Device::Get()->GetPipelineCache()->GetPipeline(this->m_pipelineKey);
        cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline->GetPipeline());

        vk::Viewport viewport {
            0.f,
            0.f,
            static_cast<float>(this->m_renderPass->GetExtent().width),
            static_cast<float>(this->m_renderPass->GetExtent().height),
            0.0f,
            1.0f
        };
        cmd.setViewport(0, viewport);
        cmd.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), this->m_renderPass->GetExtent()));

        struct MVP_PickID
        {
            std::array<float, 16> mvp {};
            std::array<float, 3> id {};
        };

        std::array<MVP_PickID, 1> pushConstants {};
        std::array<vk::Buffer, 1> vertexBuffers {};

        auto& camera = this->m_renderer->GetCamera();
        auto VPMat   = Multi(camera->GetViewMatrix(), camera->GetProjectMatrix());

        uint32_t index {1};
        for (const auto& actor : this->m_renderer->GetAllActors())
        {
            if (!actor->GetVisibility())
            {
                continue;
            }

            auto r = index & 0xFF;
            auto g = index >> 8 & 0xFF;
            auto b = index >> 16 & 0xFF;

            id_actors.try_emplace(index++, actor);

            pushConstants[0].mvp = Multi(actor->GetModelMatrix(), VPMat);
            pushConstants[0].id  = {r / 255.f, g / 255.f, b / 255.f};

            auto& drawable   = actor->GetMapper()->GetDrawable();
            vertexBuffers[0] = drawable->GetVertexPosBuffer();

            cmd.pushConstants<MVP_PickID>(
                pipeline->GetPipelineLayout(),
                vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                0,
                pushConstants
            );

            cmd.bindVertexBuffers(0, vertexBuffers, {0});
            cmd.bindIndexBuffer(drawable->GetIndexBuffer(), 0, drawable->GetIndexType());
            cmd.drawIndexed(drawable->GetIndexCount(), 1, 0, 0, 0);
        }

        cmd.endRenderPass();
    });

    if (!id_actors.empty())
    {
        auto id = GetActorID(displayPos);
        if (id_actors.contains(id))
        {
            return id_actors.at(id);
        }
    }

    return {};
}

uint32_t Picker::GetActorID(const std::array<int, 2>& displayPos) const noexcept
{
    auto& extent = m_renderPass->GetExtent();

    ImageData imageData(
        vk::Format::eB8G8R8A8Unorm,
        m_renderPass->GetExtent(),
        vk::ImageTiling::eLinear,
        vk::ImageUsageFlagBits::eTransferDst,
        vk::ImageLayout::eUndefined,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        vk::ImageAspectFlagBits::eColor,
        vk::SampleCountFlagBits::e1,
        false
    );

    MemoryHelper::OneTimeSubmit([&imageData, &extent, this](const vk::raii::CommandBuffer& cmd) {
        auto format     = vk::Format::eB8G8R8A8Unorm;
        auto&& inImage  = this->m_renderPass->GetColorImageData()->GetImage();
        auto&& outImage = imageData.GetImage();

        ImageData::SetImageLayout(
            cmd, outImage, format, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal
        );

        auto formatProperties = Device::Get()->GetPhysicalDevice().getFormatProperties(format);

        if ((formatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eBlitSrc)
            && (formatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eBlitDst))
        {
            vk::ImageSubresourceLayers imageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
            std::array<vk::Offset3D, 2> offsets {
                vk::Offset3D(0, 0, 0),
                vk::Offset3D(static_cast<int32_t>(extent.width), static_cast<int32_t>(extent.height), 1)
            };
            vk::ImageBlit imageBlit(imageSubresourceLayers, offsets, imageSubresourceLayers, offsets);
            cmd.blitImage(
                inImage,
                vk::ImageLayout::eTransferSrcOptimal,
                outImage,
                vk::ImageLayout::eTransferDstOptimal,
                imageBlit,
                vk::Filter::eLinear
            );
        }
        else
        {
            vk::ImageSubresourceLayers imageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1);
            vk::ImageCopy imageCopy(
                imageSubresourceLayers, vk::Offset3D(), imageSubresourceLayers, vk::Offset3D(), vk::Extent3D(extent, 1)
            );
            cmd.copyImage(
                inImage, vk::ImageLayout::eTransferSrcOptimal, outImage, vk::ImageLayout::eTransferDstOptimal, imageCopy
            );
        }

        ImageData::SetImageLayout(
            cmd, outImage, format, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eGeneral
        );
    });

    auto layout = imageData.GetImage().getSubresourceLayout({vk::ImageAspectFlagBits::eColor, 0, 0});
    auto pixels = reinterpret_cast<uint8_t*>(imageData.GetDeviceMemory().mapMemory(0, vk::WholeSize, {}));
    pixels += layout.offset;
    pixels += static_cast<uint64_t>(displayPos[1]) * layout.rowPitch + static_cast<uint64_t>(displayPos[0]) * 4;

    uint8_t pickedR = *(pixels + 2);
    uint8_t pickedG = *(pixels + 1);
    uint8_t pickedB = *(pixels + 0);

    uint32_t id {};
    id |= pickedR;
    id |= pickedG << 8;
    id |= pickedB << 16;

    imageData.GetDeviceMemory().unmapMemory();

    return id;
}
