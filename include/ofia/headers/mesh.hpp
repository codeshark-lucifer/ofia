#pragma once
#include "buffers/vao.hpp"
#include "buffers/vbo.hpp"
#include "buffers/ebo.hpp"

#include <memory>
#include <stdexcept>
#include <ofia/headers/shader.hpp>
#include <ofia/headers/texture.hpp>

namespace ofia
{
    class Mesh
    {
    public:
        Mesh(
            const std::vector<Vertex> &vertices,
            const std::vector<unsigned int> &indices,
            const std::vector<std::shared_ptr<Texture>> &textures)
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
            if (this->textures.empty())
            {
                this->textures.push_back(std::make_unique<Texture>("assets/textures/default_sprite.png", TextureType::Diffuse, true));
            }

            vao = std::make_unique<VAO>();
            vao->Bind(); // Bind VAO first

            vbo = std::make_unique<VBO>();
            vbo->Bind();                                  // Bind VBO
            vbo->SetData(this->vertices, GL_STATIC_DRAW); // Set VBO data

            ebo = std::make_unique<EBO>();
            ebo->Bind();                                 // Bind EBO
            ebo->SetData(this->indices, GL_STATIC_DRAW); // Set EBO data

            Setup();       // Configure attributes for the bound VAO
            vao->Unbind(); // Unbind VAO (safe practice after setup)
        }

        void Draw(const Shader &shader) const
        {
            // shader.Use(); // Redundant, already called by MeshRenderer::Render()

            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;

            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i);

                std::string name;
                switch (textures[i]->type)
                {
                case TextureType::Diffuse:
                    name = "texture_diffuse" + std::to_string(diffuseNr++);
                    break;
                case TextureType::Specular:
                    name = "texture_specular" + std::to_string(specularNr++);
                    break;
                default:
                    name = "texture_unknown";
                    break;
                }

                shader.SetUniform(name, (int)i);
                textures[i]->Bind(i);
            }

            glActiveTexture(GL_TEXTURE0);

            vao->Bind();
            glDrawElements(GL_TRIANGLES, ebo->GetSize(), GL_UNSIGNED_INT, 0);
            // vao->Unbind(); // No need to unbind here, it's safer to keep it bound during drawing and let the next draw call bind its VAO.
        }

    private:
        void Setup()
        {
            if (!vao || !vbo || !ebo)
                throw std::runtime_error("Failed: some buffer is nullptr.");

            // VAO, VBO, EBO are already bound by the constructor
            // No need to bind again here

            // Vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(
                0,                                 // attribute 0
                3,                                 // vec3
                GL_FLOAT,                          // type
                GL_FALSE,                          // normalized?
                sizeof(Vertex),                    // stride (size of one vertex)
                (void *)offsetof(Vertex, Position) // offset
            );

            // Vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (void *)offsetof(Vertex, Normal));

            // Vertex texture coordinates
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(
                2,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Vertex),
                (void *)offsetof(Vertex, TexCoords));

            // Unbind VAO is now handled in the constructor after setup.
        }
        std::unique_ptr<VAO> vao;
        std::unique_ptr<VBO> vbo;
        std::unique_ptr<EBO> ebo;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<Texture>> textures;
    };
} // namespace ofia
