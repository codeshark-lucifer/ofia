#include "ofia/headers/scene.hpp"

#include <ofia/ofia.h>
#include <ofia/headers/components/meshrenderer.hpp>
#include <ofia/headers/components/light.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ofia
{
    Scene::Scene(
        const std::shared_ptr<Camera> &camera,
        const std::shared_ptr<Shader> &shader)
        : defaultCamera(camera), defaultShader(shader)
    {
        defaultShader->Use();
        defaultShader->SetUniform("objectColor", glm::vec3(1.0f));
        defaultQuad = std::make_unique<Quad>();

        shadowbuffer = std::make_unique<SBO>(SHADOW_WIDTH, SHADOW_HEIGHT);
        shadowbufferShader = std::make_shared<Shader>("assets/shaders/glsl/depth.glsl");
        framebufferShader = std::make_shared<Shader>(
            "assets/shaders/vert/render.vert",
            "assets/shaders/frag/fbo.frag");

        framebufferShader->Use();
        framebufferShader->SetUniform("screenTexture", 0);

        // Create single FBO that contains MSAA + resolved texture
        multisampleFBO = std::make_unique<FBO>(WINDOW_WIDTH, WINDOW_HEIGHT, 8);
    }

    void Scene::Awake()
    {
        for (auto &go : gameObjects)
            go->Awake();
    }

    void Scene::Start()
    {
        for (auto &go : gameObjects)
            go->Start();
    }

    void Scene::Update(float dt)
    {
        for (auto &go : gameObjects)
            go->Update(dt);
    }

    void Scene::FixedUpdate(float dt)
    {
        for (auto &go : gameObjects)
            go->FixedUpdate(dt);
    }

    void Scene::LateUpdate(float dt)
    {
        for (auto &go : gameObjects)
            go->LateUpdate(dt);
    }

    void Scene::Render()
    {

        for (auto &go : gameObjects)
        {
            auto lights = go->GetComponents<Light>();
            for (auto &light : lights)
            {
                glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 20.0f);
                glm::mat4 lightView = glm::lookAt(light->position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                lightSpaceMatrix = lightProjection * lightView;
            }
        }

        shadowbuffer->Bind();
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowbufferShader->Use();
        shadowbufferShader->SetUniform("lightSpaceMatrix", lightSpaceMatrix);

        for (auto &go : gameObjects)
        {
            if (auto m = go->GetComponent<MeshRenderer>())
            {
                shadowbufferShader->SetUniform("model", go->transform->GetWorldMatrix());
                m->Render(*shadowbufferShader);
            }
        }

        shadowbuffer->Unbind();

        // 1. Render 3D scene into FBO
        multisampleFBO->Bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(pow(linearBackground, gamma),
                     pow(linearBackground, gamma),
                     pow(linearBackground, gamma), 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        defaultShader->Use();
        defaultShader->SetUniform("view", defaultCamera->GetViewMatrix());
        defaultShader->SetUniform("projection", defaultCamera->GetProjectionMatrix());
        defaultShader->SetUniform("viewPos", defaultCamera->position);
        defaultShader->SetUniform("material.ambient", glm::vec3(0.2f));
        defaultShader->SetUniform("material.diffuse", glm::vec3(0.6f));
        defaultShader->SetUniform("material.specular", glm::vec3(1.0f));
        defaultShader->SetUniform("material.shininess", 32.0f);
        defaultShader->SetUniform("lightSpaceMatrix", lightSpaceMatrix);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, shadowbuffer->GetTexture());
        defaultShader->SetUniform("shadowMap", 1);
        
        // Gather all mesh renderers
        std::vector<std::shared_ptr<MeshRenderer>> renderers;
        for (auto &go : gameObjects)
        {
            if (!go)
                continue;

            auto lights = go->GetComponents<Light>();
            for (auto &light : lights)
            {
                defaultShader->SetUniform("lightPosition", light->position);
                defaultShader->SetUniform("lightColor", light->color);
            }

            auto meshes = go->GetComponentsRecursive<MeshRenderer>();
            renderers.insert(renderers.end(), meshes.begin(), meshes.end());
        }

        // Draw meshes
        for (auto &r : renderers)
            if (r)
                r->Render(*defaultShader);

        multisampleFBO->Unbind();

        // Draw the FBO texture to screen
        glDisable(GL_DEPTH_TEST);
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        framebufferShader->Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, multisampleFBO->GetTexture());
        framebufferShader->SetUniform("gammaCorrected", gamma);

        defaultQuad->Draw();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Scene::OnResize(int width, int height)
    {
        defaultCamera->Resize(width, height);
        multisampleFBO->Resize(width, height);
        shadowbuffer->Resize(width, height);
    }

    std::shared_ptr<GameObject> Scene::CreateGameObject(const char *name)
    {
        auto go = std::make_shared<GameObject>(name);
        gameObjects.push_back(go);
        return go;
    }

    void Scene::AddGameObject(const std::shared_ptr<GameObject> &obj)
    {
        gameObjects.push_back(obj);
    }
}
