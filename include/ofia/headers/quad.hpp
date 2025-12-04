#pragma once
#include <glad/glad.h>

namespace ofia
{
    class Quad
    {
    public:
        Quad() { Setup(); }
        ~Quad() { 
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
        }

        void Draw() const
        {
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

    private:
        unsigned int VAO = 0, VBO = 0;

        void Setup()
        {
            // Fullscreen quad positions and texcoords
            float vertices[] = {
                // positions       // texcoords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right

                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // top-left
                 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
                 1.0f,  1.0f, 0.0f, 1.0f, 1.0f  // top-right
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // texcoord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
        }
    };
}
