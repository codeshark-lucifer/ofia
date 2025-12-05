#pragma once
#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ofia/headers/camera.hpp>
#include <ofia/headers/shader.hpp>
#include <ofia/headers/components/gameobject.hpp>
#include <ofia/headers/quad.hpp>
#include <ofia/headers/buffers/fbo.hpp>
#include <ofia/headers/buffers/sbo.hpp>

namespace ofia
{
    class Scene
    {
    public:
        Scene(
            const std::shared_ptr<Camera> &camera,
            const std::shared_ptr<Shader> &shader);
        ~Scene() = default;

        void Awake();
        void Start();
        void Update(float dt);
        void FixedUpdate(float dt);
        void LateUpdate(float dt);
        void Render();
        void OnResize(int width, int height);

        std::shared_ptr<GameObject> CreateGameObject(const char *name);
        void AddGameObject(const std::shared_ptr<GameObject> &obj);
        std::vector<std::shared_ptr<GameObject>> GetGameObjects() { return gameObjects; }

    private:
        float gamma = 2.2f;
        float linearBackground = 0.1f;
        glm::mat4 lightSpaceMatrix;
        std::vector<std::shared_ptr<GameObject>> gameObjects;

        std::shared_ptr<Camera> defaultCamera = nullptr;
        std::shared_ptr<Shader> defaultShader = nullptr;
        std::shared_ptr<Shader> framebufferShader = nullptr;
        std::shared_ptr<Shader> shadowbufferShader = nullptr;

        std::unique_ptr<Quad> defaultQuad = nullptr;

        // MSAA FBO (this FBO contains both the multisampled and resolved textures)
        std::unique_ptr<FBO> multisampleFBO = nullptr;
        std::unique_ptr<SBO> shadowbuffer = nullptr;
    };
}
