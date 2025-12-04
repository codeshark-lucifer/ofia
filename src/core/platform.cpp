#include "ofia/headers/platform.hpp"
#include <stdexcept>
#include <glad/glad.h> // Include glad.h here for gladLoadGLLoader

namespace ofia
{
    Platform::Platform(int width, int height, const std::string& title)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!m_Window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window.");
        }

        glfwMakeContextCurrent(m_Window);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD.");
        }
        
        glfwSwapInterval(1); // vsync
    }

    Platform::~Platform()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    bool Platform::ShouldClose() const
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Platform::PollEvents() const
    {
        glfwPollEvents();
    }

    void Platform::SwapBuffers() const
    {
        glfwSwapBuffers(m_Window);
    }
    
}
