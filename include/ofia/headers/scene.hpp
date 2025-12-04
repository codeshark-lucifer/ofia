#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <ofia/headers/camera.hpp>
#include <ofia/headers/shader.hpp>
#include <ofia/headers/components/gameobject.hpp>

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

        std::shared_ptr<GameObject> CreateGameObject(const char* name){
            auto go = std::make_shared<GameObject>(name);
            gameObjects.push_back(go);
            return go;
        }
        void AddGameObject(const std::shared_ptr<GameObject>& obj){
            gameObjects.push_back(obj);
        }

    private:
        std::vector<std::shared_ptr<GameObject>> gameObjects;
        std::shared_ptr<Camera> defaultCamera;
        std::shared_ptr<Shader> defaultShader;
    };
}
