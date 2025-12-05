#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ofia
{
    class GameObject; // forward declaration

    class Transform
    {
    public:
        GameObject *gameobject = nullptr;

        glm::vec3 position{0.0f};
        glm::vec3 rotation{0.0f}; // Euler angles in radians
        glm::vec3 scale{1.0f};

        Transform();
        Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &sca);

        // Local model matrix (relative to parent)
        glm::mat4 GetLocalMatrix() const;

        // World matrix (recursive: parent * local)
        glm::mat4 GetWorldMatrix() const;

        // Set position/rotation/scale from an existing 4x4 matrix
        void SetFromMatrix(const glm::mat4 &mat);

        // Basic transformations
        void Translate(const glm::vec3 &delta);
        void Rotate(const glm::vec3 &delta);
        void SetScale(const glm::vec3 &newScale);

        void SetPosition(const glm::vec3 &newPos);

        void SetRotation(const glm::vec3 &newRot);
        // Optional: cached local matrix
        void UpdateLocalMatrix();

    private:
        glm::mat4 localMatrix{1.0f};
    };
}
