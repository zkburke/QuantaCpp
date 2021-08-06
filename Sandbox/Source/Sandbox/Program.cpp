#include <iostream>
#include <fstream>
#include <sstream>
#include <Quanta/Quanta.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>

#define TO_RADIANS (static_cast<float>(M_PI) / 180.0f) 

class Camera final
{
public:
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 GetView()
    {
        return glm::lookAt(Position, Position + Front, { 0.0f, 1.0f, 0.0f });
    }
};

std::string ReadAllText(const std::string& filepath)
{
    std::stringstream contents;

    std::ifstream file;

    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        file.open(filepath);
        {
            contents << file.rdbuf();
        }
        file.close();
    }
    catch(const std::exception& exception)
    {
        std::cerr << exception.what() << '\n';
    }
    
    return std::move(contents.str());
}

int main()
{
    std::shared_ptr<Quanta::Window> window = Quanta::Window::Create(Quanta::GraphicsApi::OpenGL);
    
    Quanta::GraphicsDevice::Initialize(window);
    
    Quanta::AudioDevice::Initialize(Quanta::AudioApi::OpenAL);

    window->SetState(Quanta::WindowState::Maximized);

    Quanta::ImGuiRenderer::Initialize(*window);

    std::vector<std::shared_ptr<Quanta::Image32>> images =
    {
        Quanta::Image32::FromFile("Resources/Textures/Skybox/right.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/left.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/top.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/bottom.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/back.png"),
        Quanta::Image32::FromFile("Resources/Textures/Skybox/front.png")
    };
    
    std::shared_ptr<Quanta::CubeMap> cubeMap = Quanta::CubeMap::FromImages(images);

    std::shared_ptr<Quanta::SamplerCube> cubeSampler = Quanta::SamplerCube::Create(cubeMap);

    cubeSampler->SetIsSeamless(true);
    
    float time = 0;

    Quanta::Renderer3D::Initialize(*window);

    std::shared_ptr<Quanta::Texture2D> albedo = Quanta::Texture2D::FromFile("Resources/Models/backpack/diffuse.jpg");
    std::shared_ptr<Quanta::Texture2D> diffuse = Quanta::Texture2D::FromFile("Resources/Models/backpack/roughness.jpg");
    std::shared_ptr<Quanta::Texture2D> specular = Quanta::Texture2D::FromFile("Resources/Models/backpack/specular.jpg");
    std::shared_ptr<Quanta::Texture2D> normal = Quanta::Texture2D::FromFile("Resources/Models/backpack/normal.png");

    std::shared_ptr<Quanta::Sampler2D> albedoSampler = Quanta::Sampler2D::Create(albedo);
    std::shared_ptr<Quanta::Sampler2D> diffuseSampler = Quanta::Sampler2D::Create(diffuse);
    std::shared_ptr<Quanta::Sampler2D> specularSampler = Quanta::Sampler2D::Create(specular);
    std::shared_ptr<Quanta::Sampler2D> normalSampler = Quanta::Sampler2D::Create(normal);
        
    albedoSampler->SetMagnification(Quanta::FilterMode::Nearest);
    albedoSampler->SetMinification(Quanta::FilterMode::Nearest);

    Quanta::Mesh mesh = Quanta::Mesh::FromFile("Resources/Models/backpack/backpack.fbx");

    Quanta::Material material;

    material.SetAlbedo({ 1.0f, 1.0f, 1.0f, 1.0f });
    material.SetAlbedoSampler(albedoSampler.get()); 

    material.SetDiffuse({ 0.8f, 0.8f, 0.8f, 1.0f });
    material.SetDiffuseSampler(diffuseSampler.get());

    material.SetSpecular({ 0.5f, 0.5f, 0.5f, 1.0f });
    material.SetSpecularSampler(specularSampler.get());
    
    material.SetNormalSampler(normalSampler.get());

    material.SetShininess(225.0f);

    glm::vec3 pos = { 0.0f, 0.0f, 0.0f }; 
    glm::vec3 rot = { 0.0f, 0.0f, 0.0f };

    Camera camera;

    camera.Position = { 0.0f, 0.0f, 5.0f };
    camera.Front = { 0.0f, 0.0f, -5.0f };
    
    std::vector<Quanta::PointLight> lights(1);
    
    lights[0].Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
    lights[0].Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
    lights[0].Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    lights[0].Position = glm::vec3(0.0f, 0.0f, 1.5f);
    lights[0].Constant = 1.0f;
    lights[0].Linear = 0.007f;
    lights[0].Quadratic = 0.0002f;

    while(window->Exists())
    {
        time += 0.0167f;
        
        window->PollEvents();

        glm::mat4 model = glm::mat4(1.0);

        model = glm::scale(model, glm::vec3(1.0f));
        model = glm::translate(model, pos);
        model *= glm::toMat4(glm::quat(rot));
        
        Quanta::Renderer3D::BeginPass();
        {
            Quanta::Renderer3D::SetView(camera.GetView(), camera.Position);

            Quanta::Renderer3D::SetPointLights(lights.data(), lights.size());
            
            Quanta::Renderer3D::DrawMesh(mesh, material, model);
        }
        Quanta::Renderer3D::EndPass();

        Quanta::ImGuiRenderer::Begin(1.0f / 60.0f);
        {
            ImGui::ShowMetricsWindow();

            ImGui::DragFloat3("Pos", &pos.x, 0.025f);
            ImGui::DragFloat3("Rot", &rot.x, 0.025f);

            ImGui::Spacing();

            for(size_t i = 0; i < lights.size(); i++)
            {
                Quanta::PointLight& light = lights[i];

                ImGui::PushID(i);

                ImGui::Text("Light %u", static_cast<uint32_t>(i));

                ImGui::DragFloat3("Pos", &light.Position.x, 0.025f);

                ImGui::PopID();
            }

            ImGui::Spacing();

            ImGui::DragFloat3("CamPos", &camera.Position.x, 0.025f);
            ImGui::DragFloat3("CamRot", &camera.Rotation.x, 0.025f);

            ImGui::Begin("Sampler2D");
            {
                if(ImGui::Button("Toggle Filter Mode"))
                {
                    if(albedoSampler->GetMagnification() == Quanta::FilterMode::Linear)
                    {
                        albedoSampler->SetMagnification(Quanta::FilterMode::Nearest);
                        albedoSampler->SetMinification(Quanta::FilterMode::Nearest);
                    }
                    else
                    {
                        albedoSampler->SetMagnification(Quanta::FilterMode::Linear);
                        albedoSampler->SetMinification(Quanta::FilterMode::Linear);
                    }
                }
            }
            ImGui::End();
        }
        Quanta::ImGuiRenderer::End();

        window->SwapBuffers();

        Quanta::GraphicsDevice::SetRasterPipeline(nullptr);

        Quanta::GraphicsDevice::ClearBackBuffer({ 0.0f, 0.0f, 0.0f, 0.0f }, 1.0f, 0);
    }
    
    Quanta::Renderer3D::DeInitialize();

    Quanta::ImGuiRenderer::Shutdown();

    Quanta::GraphicsDevice::DeInitialize();
    Quanta::AudioDevice::DeInitialize();

    return 0;
}