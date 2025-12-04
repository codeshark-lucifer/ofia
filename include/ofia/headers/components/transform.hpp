#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ofia {
    class GameObject;  // forward declare

    class Transform
    {
    public:
        GameObject* gameobject = nullptr;

        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        Transform();
        Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& sca);

        glm::mat4 GetModelMatrix() const;
        glm::mat4 GetWorldMatrix() const;

        void Translate(const glm::vec3& delta);
        void Rotate(const glm::vec3& delta);
        void SetScale(const glm::vec3& newScale);
    };
}
