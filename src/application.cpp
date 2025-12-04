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
        glEnable(GL_DEPTH_TEST); // Enable depth testing
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Set background color to a dark teal

        auto camera = std::make_shared<Camera>(WINDOW_WIDTH, WINDOW_HEIGHT);
        auto shader = std::make_shared<Shader>("assets/shaders/basic.glsl");
        scene = std::make_unique<Scene>(camera, shader);
        
        glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
        auto light = scene->CreateGameObject("Light");
        light->AddComponent<Light>(lightPos, glm::vec3(1.0f));
        
        AssetLoader asset("assets/models/cube.fbx");
        scene->AddGameObject(asset.gameObject);
        if (asset.gameObject && asset.gameObject->transform) {
            asset.gameObject->transform->SetScale(glm::vec3(10.0f)); // Scale it up significantly
        }
    }

    void Application::Run()
    {
        LastTime = static_cast<float>(glfwGetTime());
        scene->Awake();
        scene->Start();

        while (!platform->ShouldClose())
        {
            float time = static_cast<float>(glfwGetTime());
            float dt = time - LastTime;
            LastTime = time;

            // Update transforms, physics, scripts
            scene->FixedUpdate(dt); // physics / fixed timestep
            scene->Update(dt);      // game logic, input
            scene->LateUpdate(dt);  // finalize transforms, animations

            // Render the scene
            scene->Render(); // draw everything to the screen

            // Swap buffers and poll OS events
            platform->SwapBuffers();
            platform->PollEvents();
        }
    }

}
