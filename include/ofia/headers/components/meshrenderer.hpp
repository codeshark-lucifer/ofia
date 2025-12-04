#pragma once
#include <ofia/headers/components/component.hpp>
#include <ofia/headers/components/meshfilter.hpp>

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
                shader.SetUniform("model", gameObject->transform->GetModelMatrix());
                meshfilter->mesh->Draw(shader);
            }
        }
    };
}