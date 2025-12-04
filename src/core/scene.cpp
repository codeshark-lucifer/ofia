#include "ofia/headers/scene.hpp"
#include <ofia/headers/components/meshrenderer.hpp>
#include <ofia/headers/components/light.hpp>

namespace ofia
{
    Scene::Scene(
        const std::shared_ptr<Camera> &camera,
        const std::shared_ptr<Shader> &shader) : defaultCamera(camera), defaultShader(shader) {}

    void Scene::Awake()
    {
    }

    void Scene::Start()
    {
    }

    void Scene::Update(float dt)
    {
    }

    void Scene::FixedUpdate(float dt)
    {
    }

    void Scene::LateUpdate(float dt)
    {
    }

    void Scene::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers
        defaultShader->Use(); // Activate the shader before setting uniforms

        glm::mat4 viewMatrix = defaultCamera->GetViewMatrix();
        glm::mat4 projectionMatrix = defaultCamera->GetProjectionMatrix();
        glm::vec3 cameraPosition = defaultCamera->position;

        defaultShader->SetUniform("view", viewMatrix);
        defaultShader->SetUniform("projection", projectionMatrix);
        defaultShader->SetUniform("viewPos", cameraPosition);
        defaultShader->SetUniform("objectColor", glm::vec3(1.0, 1.0, 1.0));

        // Collect all MeshRenderers from all GameObjects in the scene hierarchy
        std::vector<std::shared_ptr<MeshRenderer>> allMeshRenderers;
        for (auto &go : gameObjects)
        {
            if (!go)
                continue;
            
            // First, process lights. Currently, lights are expected to be on top-level GameObjects
            auto lightComps = go->GetComponents<Light>();
            for (auto &light : lightComps)
            {
                defaultShader->SetUniform("lightPos", light->position);
                defaultShader->SetUniform("lightColor", light->color);
            }

            // Recursively get all MeshRenderers from this GameObject and its children
            std::vector<std::shared_ptr<MeshRenderer>> currentGORenderers = go->GetComponentsRecursive<MeshRenderer>();
            allMeshRenderers.insert(allMeshRenderers.end(), currentGORenderers.begin(), currentGORenderers.end());
        }

        // Render all collected MeshRenderers
        for (auto &render : allMeshRenderers)
        {
            if (!render)
                continue;

            render->Render(*defaultShader);
        }
    }

} // namespace ofia
