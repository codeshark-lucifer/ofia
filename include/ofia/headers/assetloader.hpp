#pragma once
#include "mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include <ofia/headers/components/gameobject.hpp>
#include <ofia/headers/components/meshfilter.hpp>
#include <ofia/headers/components/meshrenderer.hpp>

namespace ofia
{
    class AssetLoader
    {
    public:
        AssetLoader(const std::string &path)
        {
            LoadModel(path);
        }
        std::shared_ptr<GameObject> gameObject = nullptr;

    private:
        std::string directory;

        void LoadModel(const std::string &path)
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path,
                                                     aiProcess_Triangulate |
                                                         aiProcess_FlipUVs |
                                                         aiProcess_CalcTangentSpace |
                                                         aiProcess_GenNormals);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                // std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl; // Debug print
                return;
            }

            directory = path.substr(0, path.find_last_of("/\\"));
            gameObject = std::make_shared<GameObject>(scene->mRootNode->mName.C_Str());
            ProcessNode(scene->mRootNode, scene);
        }

        void ProcessNode(aiNode *node, const aiScene *scene)
        {
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                std::string meshName = mesh->mName.C_Str();
                auto m = std::make_shared<Mesh>(ProcessMesh(mesh, scene));
                auto obj = std::make_shared<GameObject>(meshName.c_str());
                obj->AddComponent<MeshFilter>(m);
                obj->AddComponent<MeshRenderer>();
                if (gameObject) {
                    gameObject->AddChild(obj);
                }
            }

            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene);
            }
        }

        Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<std::shared_ptr<Texture>> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                vertex.Position = glm::vec3(mesh->mVertices[i].x,
                                            mesh->mVertices[i].y,
                                            mesh->mVertices[i].z);

                vertex.Normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x,
                                                               mesh->mNormals[i].y,
                                                               mesh->mNormals[i].z)
                                                   : glm::vec3(0.0f, 0.0f, 0.0f);

                if (mesh->mTextureCoords[0])
                    vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
                                                 mesh->mTextureCoords[0][i].y);
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            return Mesh(vertices, indices, textures);
        }
    };
}
