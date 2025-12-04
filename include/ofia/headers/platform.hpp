#pragma once

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#endif

#include <GLFW/glfw3.h>
#include <string>

namespace ofia
{
    class Platform
    {
    public:
        Platform(int width, int height, const std::string& title);
        ~Platform();

        GLFWwindow* GetWindow() const { return m_Window; }
        bool ShouldClose() const;
        void PollEvents() const;
        void SwapBuffers() const;

    private:
        GLFWwindow* m_Window = nullptr;
    };
}
