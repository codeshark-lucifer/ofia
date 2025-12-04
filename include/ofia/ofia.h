#pragma once
#include <string>
#include <ofia/headers/scene.hpp>
#include <ofia/headers/components/gameobject.hpp>

namespace ofia
{
    // ----------------------------------------
    // Engine Global Settings / Constants
    // ----------------------------------------

    // Application metadata
    static constexpr const char* APP_NAME = "ofia - engine";

    // Window size
    static constexpr int WINDOW_WIDTH  = 1280;
    static constexpr int WINDOW_HEIGHT = 720;

    // Shadow map resolution
    static constexpr int SHADOW_WIDTH  = 2048;
    static constexpr int SHADOW_HEIGHT = 2048;

    // You can add more engine-wide constants here:
    // static constexpr float NEAR_PLANE = 0.1f;
    // static constexpr float FAR_PLANE  = 1000.0f;

    // Helper function to fetch as std::string if needed
    inline std::string GetAppName() { return APP_NAME; }
}
