#include "ofia/headers/camera.hpp"

namespace ofia
{
    Camera::Camera(float width, float height)
    {
        aspectRatio = width / height;
        front = glm::vec3(0.0f, 0.0f, -1.0f);
        worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        up = worldUp;
        right = glm::normalize(glm::cross(front, worldUp));
        target = position + front;
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(position, target, up);
    }

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    }

    void Camera::Resize(float width, float height)
    {
        if (height == 0)
            height = 1;
        aspectRatio = width / height;
    }

    void Camera::Process(float deltaTime, GLFWwindow *window)
    {
        // Toggle right mouse
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            rightMousePressed = true;
        else
            rightMousePressed = false;

        if (!rightMousePressed)
        {
            firstMouse = true; // reset mouse
            return;
        }

        // Mouse movement
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = static_cast<float>(xpos - lastX);
        float yoffset = static_cast<float>(lastY - ypos); // reversed
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateCameraVectors();

        // Keyboard movement
        float velocity = speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position += front * velocity;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position -= front * velocity;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position -= right * velocity;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position += right * velocity;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            position += worldUp * velocity;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            position -= worldUp * velocity;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            position -= front * velocity * 0.5f; // optional custom behavior
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            position += front * velocity * 0.5f;

        target = position + front;
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }

} // namespace ofia
