#include "ofia/headers/scene.hpp"

#include <ofia/ofia.h>
#include <ofia/headers/components/meshrenderer.hpp>
#include <ofia/headers/components/light.hpp>

namespace ofia
{
    Scene::Scene(
        const std::shared_ptr<Camera> &camera,
        const std::shared_ptr<Shader> &shader) : defaultCamera(camera), defaultShader(shader)
    {
        defaultQauad = std::make_unique<Quad>();
        framebufferShader = std::make_unique<Shader>(
            "assets/shaders/vert/render.vert",
            "assets/shaders/frag/fbo.frag");
        framebuffer = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT);

        framebufferShader->Use();
        framebufferShader->SetUniform("screenTexture", 0);
    }

    void Scene::Awake()
    {
        for (auto &go : gameObjects)
        {
            go->Awake();
        }
    }

    void Scene::Start()
    {
        for (auto &go : gameObjects)
        {
            go->Awake();
        }
    }

    void Scene::Update(float dt)
    {
        for (auto &go : gameObjects)
        {
            go->Update(dt);
        }
    }

    void Scene::FixedUpdate(float dt)
    {
        for (auto &go : gameObjects)
        {
            go->FixedUpdate(dt);
        }
    }

    void Scene::LateUpdate(float dt)
    {
        for (auto &go : gameObjects)
        {
            go->LateUpdate(dt);
        }
    }

    void Scene::Render()
    {
        //
        // 1. Render 3D world into framebuffer
        //
        framebuffer->Bind();
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        defaultShader->Use();

        glm::mat4 viewMatrix = defaultCamera->GetViewMatrix();
        glm::mat4 projectionMatrix = defaultCamera->GetProjectionMatrix();
        glm::vec3 cameraPosition = defaultCamera->position;

        defaultShader->SetUniform("view", viewMatrix);
        defaultShader->SetUniform("projection", projectionMatrix);
        defaultShader->SetUniform("viewPos", cameraPosition);
        defaultShader->SetUniform("objectColor", glm::vec3(1.0f));

        // Collect all MeshRenderers
        std::vector<std::shared_ptr<MeshRenderer>> allMeshRenderers;

        for (auto &go : gameObjects)
        {
            if (!go)
                continue;

            // Lights
            auto lightComps = go->GetComponents<Light>();
            for (auto &light : lightComps)
            {
                defaultShader->SetUniform("lightPos", light->position);
                defaultShader->SetUniform("lightColor", light->color);
            }

            // MeshRenderers
            auto renders = go->GetComponentsRecursive<MeshRenderer>();
            allMeshRenderers.insert(allMeshRenderers.end(), renders.begin(), renders.end());
        }

        // Render meshes
        for (auto &render : allMeshRenderers)
        {
            if (render)
                render->Render(*defaultShader);
        }

        framebuffer->Unbind();

        //
        // 2. Render fullscreen quad with framebuffer texture
        //
        glDisable(GL_DEPTH_TEST); // important!

        framebufferShader->Use();
        glActiveTexture(GL_TEXTURE0); // use sampler0
        glBindTexture(GL_TEXTURE_2D, framebuffer->GetTexture());

        defaultQauad->Draw();
    }

    void Scene::OnResize(int width, int height)
    {
        framebuffer->Resize(width, height);
    }

} // namespace ofia
