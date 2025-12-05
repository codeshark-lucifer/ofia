#include "ofia/headers/components/transform.hpp"
#include "ofia/headers/components/gameobject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ofia
{
    Transform::Transform() = default;

    Transform::Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &sca)
        : position(pos), rotation(rot), scale(sca)
    {
        UpdateLocalMatrix();
    }

    void Transform::Translate(const glm::vec3 &delta)
    {
        position += delta;
        UpdateLocalMatrix();
    }

    void Transform::Rotate(const glm::vec3 &delta)
    {
        rotation += delta;
        UpdateLocalMatrix();
    }

    void Transform::SetPosition(const glm::vec3 &newPos)
    {
        position = newPos;
        UpdateLocalMatrix();
    }

    void Transform::SetRotation(const glm::vec3 &newRot)
    {
        rotation = newRot;
        UpdateLocalMatrix();
    }

    void Transform::SetScale(const glm::vec3 &newScale)
    {
        scale = newScale;
        UpdateLocalMatrix();
    }

    void Transform::UpdateLocalMatrix()
    {
        glm::mat4 t = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 r = glm::toMat4(glm::quat(rotation)); // rotation from Euler
        glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);
        localMatrix = t * r * s;
    }

    glm::mat4 Transform::GetLocalMatrix() const
    {
        return localMatrix;
    }

    glm::mat4 Transform::GetWorldMatrix() const
    {
        if (gameobject)
        {
            if (auto parentShared = gameobject->parent.lock()) // lock weak_ptr
                return parentShared->transform->GetWorldMatrix() * localMatrix;
        }
        return localMatrix;
    }

    void Transform::SetFromMatrix(const glm::mat4 &mat)
    {
        // Extract scale
        scale.x = glm::length(glm::vec3(mat[0]));
        scale.y = glm::length(glm::vec3(mat[1]));
        scale.z = glm::length(glm::vec3(mat[2]));

        // Extract position
        position = glm::vec3(mat[3]);

        // Extract rotation (assumes uniform scale)
        glm::mat3 rotMat;
        rotMat[0] = glm::vec3(mat[0]) / scale.x;
        rotMat[1] = glm::vec3(mat[1]) / scale.y;
        rotMat[2] = glm::vec3(mat[2]) / scale.z;

        rotation = glm::eulerAngles(glm::quat_cast(rotMat));
        UpdateLocalMatrix();
    }
}
