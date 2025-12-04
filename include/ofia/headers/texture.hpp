#include <string>
#include <glad/glad.h>
#include <ofia/debug.hpp>

namespace ofia
{
    enum class TextureType
    {
        Diffuse,
        Specular,
        Normal,
        Metalness,
        Roughness,
        AO
    };

    class Texture
    {
    public:
        unsigned int ID = 0; // Initialize to 0
        int width = 0, height = 0, nrChannels = 0;
        std::string filepath;
        TextureType type;

        Texture() = default; // Default constructor

        // Constructor: load texture from file and set type
        Texture(const std::string &path, TextureType type = TextureType::Diffuse, bool gammaCorrection = false);

        // Delete copy constructor and assignment operator
        Texture(const Texture &) = delete;
        Texture &operator=(const Texture &) = delete;

        // Move constructor and assignment operator
        Texture(Texture &&other) noexcept;
        Texture &operator=(Texture &&other) noexcept;

        // Bind the texture
        void Bind(unsigned int unit = 0) const;

        // Delete the texture
        ~Texture();
    };

}