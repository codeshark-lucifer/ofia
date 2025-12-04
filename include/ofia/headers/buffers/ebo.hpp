#pragma once
#include <glad/glad.h>
#include <vector>
#include <cstdint>

namespace ofia
{
    class EBO
    {
    public:
        EBO()
        {
            glGenBuffers(1, &ID);
        }

        ~EBO()
        {
            glDeleteBuffers(1, &ID);
        }

        void Bind() const
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        }

        void SetData(const std::vector<unsigned int> &data, GLenum usage = GL_STATIC_DRAW)
        {
            Bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(unsigned int), data.data(), usage);
            size = static_cast<int>(data.size());
        }

        int GetSize() const { return size; }

    private:
        unsigned int ID;
        int size = 0;
    };
}