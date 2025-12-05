#include "ofia/headers/buffers/fbo.hpp"
namespace ofia
{
    FBO::FBO(int w, int h, unsigned int s)
        : width(w), height(h), samples(s)
    {
        Init();
    }

    FBO::~FBO()
    {
        glDeleteFramebuffers(1, &FBO_ID);
        glDeleteTextures(1, &texture);
        glDeleteRenderbuffers(1, &RBO_ID);

        glDeleteFramebuffers(1, &postProcessingFBO);
        glDeleteTextures(1, &postProcessingTexture);
    }

    void FBO::Init()
    {
        // ================= MULTISAMPLED FBO =================
        glGenFramebuffers(1, &FBO_ID);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);

        // ---- MSAA color buffer ----
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA16F, width, height, GL_TRUE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

        // ---- MSAA depth+stencil ----
        glGenRenderbuffers(1, &RBO_ID);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO_ID);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO_ID);

        // ---- MUST set draw buffers ----
        GLenum attachments[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, attachments);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR: MSAA FBO incomplete!" << std::endl;

        // ================= POST-PROCESSING FBO =================
        glGenFramebuffers(1, &postProcessingFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

        glGenTextures(1, &postProcessingTexture);
        glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

        // Again MUST set draw buffers
        glDrawBuffers(1, attachments);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR: postProcessing FBO incomplete!" << std::endl;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::Bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO_ID);
        glViewport(0, 0, width, height);
    }

    void FBO::Unbind()
    {
        // resolve MSAA → normal texture
        glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO_ID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);

        glBlitFramebuffer(
            0, 0, width, height,
            0, 0, width, height,
            GL_COLOR_BUFFER_BIT,
            GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FBO::Resize(int w, int h)
    {
        if (w == width && h == height)
            return;

        width = w;
        height = h;

        glDeleteTextures(1, &texture);
        glDeleteRenderbuffers(1, &RBO_ID);
        glDeleteFramebuffers(1, &FBO_ID);

        glDeleteTextures(1, &postProcessingTexture);
        glDeleteFramebuffers(1, &postProcessingFBO);

        Init();
    }
} // namespace ofia
