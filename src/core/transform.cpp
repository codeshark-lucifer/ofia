#include "ofia/headers/components/transform.hpp"
#include "ofia/headers/components/gameobject.hpp" // full definition
#include <iostream>                               // For logging
#include <glm/gtx/string_cast.hpp>                // For glm::to_string

namespace ofia
{
    Transform::Transform()
        : position(0.0f), rotation(0.0f), scale(1.0f)
    {
    }

    Transform::Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &sca)
        : position(pos), rotation(rot), scale(sca)
    {
    }

    glm::mat4 Transform::GetModelMatrix() const
    {
        glm::mat4 model(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, scale);
        return model;
    }

    glm::mat4 Transform::GetWorldMatrix() const
    {
        if (!gameobject) // sanity check
            return GetModelMatrix();

        // Try to get parent
        std::shared_ptr<GameObject> parentPtr = gameobject->parent.lock();

        if (!parentPtr) // no parent
            return GetModelMatrix();

        // Recursively get parent's world matrix
        glm::mat4 parentWorld = parentPtr->transform->GetWorldMatrix();

        // Combine with local model matrix
        return parentWorld * GetModelMatrix();
    }

    void Transform::Translate(const glm::vec3 &delta) { position += delta; }
    void Transform::Rotate(const glm::vec3 &delta) { rotation += delta; }
    void Transform::SetScale(const glm::vec3 &newScale) { scale = newScale; }
}
