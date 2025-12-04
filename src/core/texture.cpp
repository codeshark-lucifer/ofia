#include "ofia/headers/texture.hpp"
#include <stb/stb_image.h>
namespace ofia
{
    Texture::Texture(const std::string &path, TextureType type, bool gammaCorrection)
        : ID(0), width(0), height(0), nrChannels(0), filepath(path), type(type)
    {
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (!data)
        {
            LOG_ERROR("Failed to load texture: %s", path.c_str());
            return;
        }

        GLenum internalFormat;
        GLenum dataFormat;

        if (nrChannels == 1)
        {
            internalFormat = GL_RED;
            dataFormat = GL_RED;
        }
        else if (nrChannels == 3)
        {
            internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
            dataFormat = GL_RGB;
        }
        else if (nrChannels == 4)
        {
            internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
            dataFormat = GL_RGBA;
        }
        else
        {
            internalFormat = GL_RGB;
            dataFormat = GL_RGB;
        }

        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
                     dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);

        // LOG_INFO("Loaded texture: %s (Size: %dx%d, Channels: %d, Gamma: %s)",
        //          path.c_str(), width, height, nrChannels, gammaCorrection ? "true" : "false");
    }

    Texture::Texture(Texture &&other) noexcept
        : ID(other.ID), width(other.width), height(other.height), nrChannels(other.nrChannels),
          filepath(std::move(other.filepath)), type(other.type)
    {
        other.ID = 0; // Invalidate the moved-from object
    }

    Texture &Texture::operator=(Texture &&other) noexcept
    {
        if (this != &other)
        {
            // Release own resources
            if (ID != 0)
            {
                glDeleteTextures(1, &ID);
                LOG_INFO("Deleted texture: %s", filepath.c_str()); // Log deletion of *this* object's resource
            }

            // Transfer ownership
            ID = other.ID;
            width = other.width;
            height = other.height;
            nrChannels = other.nrChannels;
            filepath = std::move(other.filepath);
            type = other.type;

            // Invalidate the moved-from object
            other.ID = 0;
        }
        return *this;
    }

    void Texture::Bind(unsigned int unit) const
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    Texture::~Texture()
    {
        if (ID != 0)
        {
            glDeleteTextures(1, &ID);
            // LOG_INFO("Deleted texture: %s", filepath.c_str());
        }
    }

} // namespace ofia
