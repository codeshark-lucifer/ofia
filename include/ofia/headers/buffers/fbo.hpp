#pragma once
#include <glad/glad.h>
#include <memory>

namespace ofia
{
    class FBO
    {
    public:
        // Constructor
        // multisample = true → Create a multisampled color buffer
        FBO(int width, int height, bool multisample = false, int samples = 4);

        ~FBO();

        // Bind / Unbind framebuffer
        void Bind() const;
        void Unbind() const;

        // Resize framebuffer and all attachments
        void Resize(int newWidth, int newHeight);

        // Blit (resolve) from MSAA → normal FBO
        void BlitTo(const FBO &destination) const;

        // Get color texture ID (only valid for non-multisample FBOs)
        unsigned int GetTexture() const { return colorTexture; }

        inline int Width() const { return width; }
        inline int Height() const { return height; }
        inline bool IsMSAA() const { return multisample; }

    private:
        void Create();
        void Destroy();

    private:
        unsigned int fbo = 0;

        // Non-MSAA texture
        unsigned int colorTexture = 0;

        // MSAA renderbuffer color attachment
        unsigned int colorBufferMSAA = 0;

        // Depth attachments (renderbuffer)
        unsigned int depthBuffer = 0;

        int width, height;
        bool multisample;
        int samples;
    };
}
