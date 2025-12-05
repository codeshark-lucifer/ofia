#include "ofia/headers/buffers/sbo.hpp"
#include <stdexcept>

namespace ofia
{
    SBO::SBO(unsigned int w, unsigned int h) : width(w), height(h)
    {
        Init();
    }

    SBO::~SBO()
    {
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &depthTexture);
    }

    void SBO::Init()
    {
        glGenFramebuffers(1, &FBO);

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = {1.f, 1.f, 1.f, 1.f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            throw std::runtime_error("Framebuffer not complete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void SBO::Bind() { glBindFramebuffer(GL_FRAMEBUFFER, FBO); }
    void SBO::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
    void SBO::Resize(unsigned int w, unsigned int h)
    {
        if (w == width && h == height)
            return; // no need to resize if same

        width = w;
        height = h;

        // Delete old resources
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &depthTexture);

        // Recreate framebuffer and depth texture
        Init();
    }

}
