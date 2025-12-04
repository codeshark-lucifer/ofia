#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <ofia/headers/camera.hpp>
#include <ofia/headers/shader.hpp>
#include <ofia/headers/components/gameobject.hpp>
#include <ofia/headers/quad.hpp>
#include <ofia/headers/buffers/fbo.hpp>

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

        std::shared_ptr<GameObject> CreateGameObject(const char *name)
        {
            auto go = std::make_shared<GameObject>(name);
            gameObjects.push_back(go);
            return go;
        }
        void AddGameObject(const std::shared_ptr<GameObject> &obj)
        {
            gameObjects.push_back(obj);
        }

    private:
        std::vector<std::shared_ptr<GameObject>> gameObjects;

        std::shared_ptr<Camera> defaultCamera = nullptr;
        std::shared_ptr<Shader> defaultShader = nullptr;
        std::shared_ptr<Shader> framebufferShader = nullptr;

        std::unique_ptr<FBO> framebuffer = nullptr;
        std::unique_ptr<Quad> defaultQauad = nullptr;
    };
}
