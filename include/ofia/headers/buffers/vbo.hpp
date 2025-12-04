#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>

namespace ofia
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class VBO
    {
    public:
        VBO()
        {
            glGenBuffers(1, &ID);
        }

        ~VBO()
        {
            glDeleteBuffers(1, &ID);
        }

        void Bind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, ID);
        }

        void Unbind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void SetData(const std::vector<Vertex> &data, GLenum usage = GL_STATIC_DRAW)
        {
            Bind();
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(Vertex), data.data(), usage);
        }


    private:
        unsigned int ID;
    };
}
