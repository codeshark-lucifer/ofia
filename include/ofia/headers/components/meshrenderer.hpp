#pragma once
#include <ofia/headers/components/component.hpp>
#include <ofia/headers/components/meshfilter.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

namespace ofia
{
    class MeshRenderer : public Component
    {
    public:
        ~MeshRenderer() = default;
        void Render(const Shader &shader)
        {
            auto comps = gameObject->GetComponents<MeshFilter>();
            for (auto &meshfilter : comps)
            {
                if (!meshfilter || !meshfilter->mesh)
                    continue;

                shader.Use();

                glm::mat4 world = gameObject->transform->GetWorldMatrix();
                shader.SetUniform("model", world);
                meshfilter->mesh->Draw(shader);
            }
        }
    };
}
