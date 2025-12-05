#pragma once
#include <glad/glad.h>

namespace ofia
{
    class SBO
    {
    public:
        SBO(unsigned int width, unsigned int height);
        ~SBO();

        void Bind();   // Bind FBO to render depth
        void Unbind(); // Return to default framebuffer
        unsigned int GetTexture() const { return depthTexture; }
        void Resize(unsigned int width, unsigned int height);

    private:
        unsigned int FBO;
        unsigned int depthTexture;

        unsigned int width, height;

        void Init();
    };

}
