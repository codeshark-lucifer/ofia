#pragma once
#include <ofia/headers/mesh.hpp>
#include <ofia/headers/components/component.hpp>

namespace ofia
{
    class MeshFilter : public Component
    {
    public:
        MeshFilter(std::shared_ptr<Mesh> &mesh)
        {
            this->mesh = mesh;
        }
        ~MeshFilter() = default;
        std::shared_ptr<Mesh> mesh;
    };
}