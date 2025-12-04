#include "ofia/headers/buffers/fbo.hpp"
#include <iostream>

namespace ofia
{
    FBO::FBO(int width, int height, bool multisample, int samples)
        : width(width), height(height), multisample(multisample), samples(samples)
    {
        Create();
    }

    FBO::~FBO()
    {
        Destroy();
    }

    void FBO::Create()
    {
        // Cleanup if already exists
        Destroy();

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        if (multisample)
        {
            // Create multisample color renderbuffer
            glGenRenderbuffers(1, &colorBufferMSAA);
            glBindRenderbuffer(GL_RENDERBUFFER, colorBufferMSAA);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGB8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBufferMSAA);

            // Depth-stencil renderbuffer
            glGenRenderbuffers(1, &depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        }
        else
        {
            // Create normal color texture
            glGenTextures(1, &colorTexture);
            glBindTexture(GL_TEXTURE_2D, colorTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

            // Depth-stencil renderbuffer
            glGenRenderbuffers(1, &depthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
        }

        // Check framebuffer completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "ERROR: Framebuffer not complete!" << std::endl;

        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::Destroy()
    {
        if (colorTexture) { glDeleteTextures(1, &colorTexture); colorTexture = 0; }
        if (colorBufferMSAA) { glDeleteRenderbuffers(1, &colorBufferMSAA); colorBufferMSAA = 0; }
        if (depthBuffer) { glDeleteRenderbuffers(1, &depthBuffer); depthBuffer = 0; }
        if (fbo) { glDeleteFramebuffers(1, &fbo); fbo = 0; }
    }

    void FBO::Bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void FBO::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::Resize(int newWidth, int newHeight)
    {
        if (width == newWidth && height == newHeight) return;

        width = newWidth;
        height = newHeight;

        Create();
    }

    void FBO::BlitTo(const FBO &destination) const
    {
        if (!multisample)
        {
            std::cerr << "Warning: BlitTo called on non-MSAA FBO!" << std::endl;
            return;
        }

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination.fbo);

        glBlitFramebuffer(
            0, 0, width, height,
            0, 0, destination.width, destination.height,
            GL_COLOR_BUFFER_BIT, GL_NEAREST
        );

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
