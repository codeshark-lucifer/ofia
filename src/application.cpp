#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "ofia/application.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include <ofia/headers/components/meshfilter.hpp>
#include <ofia/headers/components/meshrenderer.hpp>
#include <ofia/headers/components/light.hpp>
#include <ofia/headers/assetloader.hpp>

namespace ofia
{
    Application::Application()
    {
        platform = std::make_unique<Platform>(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME);

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glEnable(GL_DEPTH_TEST);              // Enable depth testing
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background color to a dark teal

        auto camera = std::make_shared<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT);
        auto shader = std::make_shared<Shader>("assets/shaders/glsl/basic.glsl");
        scene = std::make_unique<Scene>(camera, shader);

        glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
        auto light = scene->CreateGameObject("Light");
        light->AddComponent<Light>(lightPos, glm::vec3(1.0f));

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(platform->GetWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        AssetLoader asset("assets/models/cube.fbx");
        scene->AddGameObject(asset.root);

        AssetLoader asset1("assets/models/cube.fbx");
        scene->AddGameObject(asset1.root);

        selectedObject = asset.root;
    }

    void Application::Run()
    {
        // Store this pointer in GLFW window
        glfwSetWindowUserPointer(platform->GetWindow(), this);

        // Set framebuffer resize callback
        glfwSetFramebufferSizeCallback(platform->GetWindow(), [](GLFWwindow *window, int width, int height)
                                       {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (app && app->scene)
        {
            app->scene->OnResize(width, height);
        }
        glViewport(0, 0, width, height); });

        LastTime = static_cast<float>(glfwGetTime());
        scene->Awake();
        scene->Start();

        while (!platform->ShouldClose())
        {
            float time = static_cast<float>(glfwGetTime());
            float dt = time - LastTime;
            LastTime = time;

            scene->FixedUpdate(dt);
            scene->Update(dt);
            scene->LateUpdate(dt);

            // 1. Render 3D scene into FBO
            scene->Render();

            // 2. Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 3. Draw your editor UI (windows, inspector, hierarchy)
            DrawEditorUI();

            // 4. Render ImGui on top of the already rendered scene
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            platform->SwapBuffers();
            platform->PollEvents();
        }
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void Application::DrawEditorUI()
    {
        ImVec2 windowSize = ImGui::GetMainViewport()->Size;

        float leftWidth = windowSize.x * 0.25f;
        float rightWidth = windowSize.x * 0.25f;
        float centerWidth = windowSize.x - leftWidth - rightWidth;

        // ------------------- Hierarchy -------------------
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(leftWidth, windowSize.y));
        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        for (auto &go : scene->GetGameObjects())
        {
            // Build a stable unique label: "Name##<ptr>"
            std::string label = go->name + "##" + std::to_string(reinterpret_cast<uintptr_t>(go.get()));
            if (ImGui::Selectable(label.c_str(), selectedObject == go))
            {
                selectedObject = go;
            }
        }
        ImGui::End();

        // ------------------- Inspector (Right) -------------------
        ImGui::SetNextWindowPos(ImVec2(windowSize.x - rightWidth, 0));
        ImGui::SetNextWindowSize(ImVec2(rightWidth, windowSize.y));
        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        if (selectedObject)
        {
            ImGui::Text("Selected: %s", selectedObject->name.c_str());

            glm::vec3 pos = selectedObject->transform->position;
            glm::vec3 rot = selectedObject->transform->rotation;
            glm::vec3 scale = selectedObject->transform->scale;

            // Position
            if (ImGui::DragFloat3("Position", &pos.x, 0.1f))
                selectedObject->transform->SetPosition(pos);

            // Rotation
            if (ImGui::DragFloat3("Rotation", &rot.x, 0.1f))
                selectedObject->transform->SetRotation(rot);

            // Scale
            if (ImGui::DragFloat3("Scale", &scale.x, 0.1f))
                selectedObject->transform->SetScale(scale);

            // Light (if object has one)
            auto light = selectedObject->GetComponent<Light>();
            if (light)
            {
                ImGui::Separator();
                ImGui::ColorEdit3("Light Color", &light->color.x);
                ImGui::DragFloat3("Light Position", &light->position.x, 0.1f);
            }
        }
        else
        {
            ImGui::Text("No object selected.");
        }

        ImGui::End();
    }

}
