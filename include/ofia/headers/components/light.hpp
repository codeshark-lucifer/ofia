#pragma once
#include <glm/glm.hpp>
#include "component.hpp" // Include for Component base class

namespace ofia
{
    class Light : public Component
    {
    public:
        Light(
            const glm::vec3 &position,
            const glm::vec3 &color)
        {
            this->position = position;
            this->color = color;
        }
        ~Light() = default;

        glm::vec3 position;
        glm::vec3 color;
    };
}