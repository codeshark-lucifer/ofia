#pragma once
#include "mesh.hpp"
#include "components/transform.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <ofia/headers/components/gameobject.hpp>
#include <ofia/headers/components/meshfilter.hpp>
#include <ofia/headers/components/meshrenderer.hpp>

#include <memory>
#include <vector>
#include <string>
#include <iostream>

namespace ofia
{
    class AssetLoader
    {
    public:
        std::shared_ptr<GameObject> root = nullptr;

        AssetLoader(const std::string& path)
        {
            LoadModel(path);
        }

    private:
        void LoadModel(const std::string& path)
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path,
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace |
                aiProcess_GenNormals);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cerr << "ERROR::ASSIMP: " << importer.GetErrorString() << std::endl;
                return;
            }

            root = std::make_shared<GameObject>(scene->mRootNode->mName.C_Str());
            glm::mat4 rootMat = AssimpToGlm(scene->mRootNode->mTransformation);
            root->transform->SetFromMatrix(rootMat);

            ProcessNode(scene->mRootNode, scene, root);
        }

        void ProcessNode(aiNode* node, const aiScene* scene, std::shared_ptr<GameObject> parent)
        {
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                auto meshGO = std::make_shared<GameObject>(mesh->mName.C_Str());

                auto m = std::make_shared<Mesh>(ProcessMesh(mesh, scene));
                meshGO->AddComponent<MeshFilter>(m);
                meshGO->AddComponent<MeshRenderer>();

                glm::mat4 localMat = AssimpToGlm(node->mTransformation);
                meshGO->transform->SetFromMatrix(localMat);

                parent->AddChild(meshGO);
            }

            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                auto childGO = std::make_shared<GameObject>(node->mChildren[i]->mName.C_Str());
                glm::mat4 localMat = AssimpToGlm(node->mChildren[i]->mTransformation);
                childGO->transform->SetFromMatrix(localMat);

                parent->AddChild(childGO);
                ProcessNode(node->mChildren[i], scene, childGO);
            }
        }

        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<std::shared_ptr<Texture>> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex v;
                v.Position = glm::vec3(mesh->mVertices[i].x,
                                       mesh->mVertices[i].y,
                                       mesh->mVertices[i].z);

                v.Normal = mesh->HasNormals() 
                    ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z)
                    : glm::vec3(0.0f);

                v.TexCoords = mesh->mTextureCoords[0] 
                    ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                    : glm::vec2(0.0f);

                vertices.push_back(v);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            return Mesh(vertices, indices, textures);
        }

        glm::mat4 AssimpToGlm(const aiMatrix4x4& m)
        {
            glm::mat4 mat(1.0f);
            mat[0][0] = m.a1; mat[1][0] = m.a2; mat[2][0] = m.a3; mat[3][0] = m.a4;
            mat[0][1] = m.b1; mat[1][1] = m.b2; mat[2][1] = m.b3; mat[3][1] = m.b4;
            mat[0][2] = m.c1; mat[1][2] = m.c2; mat[2][2] = m.c3; mat[3][2] = m.c4;
            mat[0][3] = m.d1; mat[1][3] = m.d2; mat[2][3] = m.d3; mat[3][3] = m.d4;
            return mat;
        }
    };
}
