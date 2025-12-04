#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ofia
{
    class Camera
    {
    public:
        Camera(float width, float height);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

        void Resize(float width, float height);
        void Process(float deltaTime, GLFWwindow *window); // movement + mouse

        // Camera attributes
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
        glm::vec3 target; // updated automatically
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;

        float fov = 45.0f;
        float nearPlane = 0.1f;
        float farPlane = 100.0f;

    private:
        float aspectRatio;

        // Movement
        float speed = 5.0f;
        float sensitivity = 0.1f;

        // Mouse
        bool firstMouse = true;
        double lastX, lastY;
        float yaw = -90.0f;
        float pitch = 0.0f;
        bool rightMousePressed = false;

        void updateCameraVectors();
    };
} // namespace ofia
