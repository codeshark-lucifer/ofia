#pragma once
#include <glad/glad.h>
namespace ofia
{
    class VAO
    {
    public:
        VAO()
        {
            glGenVertexArrays(1, &ID);
        }

        ~VAO(){
            glDeleteVertexArrays(1, &ID);
        }

        void Bind() const
        {
            glBindVertexArray(ID);
        }

        void Unbind() const {
            glBindVertexArray(0);
        }

    private:
        unsigned int ID = 0;;
    };
}