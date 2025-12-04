#pragma once
#include <memory>
#include <ofia/ofia.h>
#include <ofia/headers/platform.hpp>
#include <ofia/headers/model.hpp>
#include <ofia/headers/camera.hpp>

namespace ofia
{
    class Application
    {
    public:
        Application();
        virtual ~Application() = default;

        void Run();

    private:
        std::unique_ptr<Platform> platform = nullptr;
        std::unique_ptr<Model> model = nullptr;
        std::unique_ptr<Scene> scene = nullptr;
        float LastTime = 0.0f;
    };
}
