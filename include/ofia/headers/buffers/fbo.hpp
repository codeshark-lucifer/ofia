#pragma once
#include <glad/glad.h>
#include <iostream>
namespace ofia
{
    class FBO
    {
    public:
        FBO(int width, int height, unsigned int samples = 4);
        ~FBO();

        void Bind();   // bind MSAA FBO
        void Unbind(); // blit to post-process FBO → screen
        void Resize(int width, int height);

        unsigned int GetTexture() const { return postProcessingTexture; }

    private:
        void Init();

    private:
        int width, height;
        unsigned int samples;

        unsigned int FBO_ID = 0;
        unsigned int texture = 0; // MSAA color texture
        unsigned int RBO_ID = 0;  // MSAA depth-stencil

        unsigned int postProcessingFBO = 0;
        unsigned int postProcessingTexture = 0;
    };
}