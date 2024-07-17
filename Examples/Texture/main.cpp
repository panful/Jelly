#include "Actor3D.h"
#include "DataSet.h"
#include "DataSetMapper.h"
#include "FloatData.h"
#include "InteractorGLFW.h"
#include "Renderer.h"
#include "Texture.h"
#include "WindowGLFW.h"
#include <iostream>
#include <memory>
#include <vector>

namespace {
// clang-format off
std::vector<float> points {
    0.f,  0.f,  0.f,
    0.f,  1.f,  0.f,
    1.f,  1.f,  0.f,
    1.f,  0.f,  0.f,
};

std::vector<float> colors {
    1.f, 0.f, 0.f,
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
    1.f, 1.f, 1.f,
};

std::vector<float> texCoords {
    0.f, 0.f,
    0.f, 1.f,
    1.f, 1.f,
    1.f, 0.f,
};

std::vector<uint32_t> triangles {
    0, 1, 2, 0, 2, 3,
};
// clang-format on
} // namespace

int main()
{
    auto pointData = std::make_shared<Jelly::FloatData>();
    pointData->SetData(std::move(points));

    auto colorData = std::make_shared<Jelly::FloatData>();
    colorData->SetData(std::move(colors));

    auto texCoordData = std::make_shared<Jelly::FloatData>();
    texCoordData->SetData(std::move(texCoords));

    auto indexData = std::make_shared<Jelly::DataArrayTemplate<uint32_t>>();
    indexData->SetData(std::move(triangles));

    auto dataSet = std::make_shared<Jelly::DataSet>();
    dataSet->SetPoints(pointData);
    dataSet->SetColors(colorData);
    dataSet->SetTexCoords(texCoordData);
    dataSet->SetIndices(indexData);

    std::vector<uint8_t> texturePixels {};
    texturePixels.reserve(100 * 100 * 4);
    for (auto x = 0u; x < 100u; ++x)
    {
        for (auto y = 0u; y < 100u; ++y)
        {
            texturePixels.emplace_back(x > 50 ? 0 : 255);
            texturePixels.emplace_back(y > 50 ? 0 : 255);
            texturePixels.emplace_back(0);
            texturePixels.emplace_back(255);
        }
    }

    auto texture = std::make_shared<Jelly::Texture>();
    texture->SetImage(vk::Extent2D {100, 100}, std::move(texturePixels));

    auto mapper = std::make_shared<Jelly::DataSetMapper>();
    mapper->SetDataSet(dataSet);
    mapper->SetTexture(texture);
    mapper->SetColor({1.f, 0.f, 1.f});
    mapper->SetColorMode(Jelly::ColorMode::Texture);

    auto actor = std::make_shared<Jelly::Actor3D>();
    actor->SetMapper(mapper);

    auto renderer = std::make_shared<Jelly::Renderer>();
    renderer->AddActor(actor);
    renderer->GetCamera()->SetCameraType(Jelly::CameraType::Perspective);
    renderer->ResetCamera();

    auto window = std::make_shared<Jelly::WindowGLFW>();
    window->AddRenderer(renderer);
    window->SetSize(800, 600);
    window->SetTitle("Test window");
    window->Render();

    auto interactor = std::make_shared<Jelly::InteractorGLFW>();
    interactor->SetWindow(window);
    interactor->Start();
}
