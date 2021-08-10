#include <Quanta/Renderer/Model.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <algorithm>

#include "../Debugging/Validation.h"

namespace Quanta
{
    std::string GetDirectory(const std::string& filepath)
    {
        size_t pos = filepath.find_last_of("/");

        return std::string::npos == pos ? "" : filepath.substr(0, pos);
    }

    Model Model::FromFile(const std::string& filepath)
    {
        Assimp::Importer importer;

        Model model;

        const aiScene* scene = importer.ReadFile(filepath, 
            aiProcess_Triangulate | 
            aiProcess_CalcTangentSpace |
            aiProcess_FlipUVs |
            aiProcess_RemoveRedundantMaterials
        ); 

        DEBUG_ASSERT(scene != nullptr);

        std::string directory = GetDirectory(filepath);

        for(size_t i = 0; i < scene->mNumMaterials; i++)
        {
            aiMaterial* materialData = scene->mMaterials[i];

            Material material;

            aiColor3D color = aiColor3D(1.0f);
            float shininess = 1.0f;

            if(materialData->Get(AI_MATKEY_COLOR_DIFFUSE, color) == aiReturn_SUCCESS)
            {
                material.SetDiffuse({ color.r, color.g, color.b });
            }
            
            if(materialData->Get(AI_MATKEY_COLOR_SPECULAR, color) == aiReturn_SUCCESS)
            {
                material.SetSpecular({ color.r, color.g, color.b });
            }

            if(materialData->Get(AI_MATKEY_SHININESS, shininess) == aiReturn_SUCCESS)
            {
                material.SetShininess(shininess);
            }

            aiString path;
            
            if(materialData->GetTexture(aiTextureType_DIFFUSE, 0, &path) == aiReturn_SUCCESS)
            {                
                std::string fullPath = directory + '/' + path.data;
                
                std::shared_ptr<Texture> albedo = Texture::Load2D(fullPath);
                
                material.SetAlbedoSampler(Sampler::Create(albedo));   
            }
            
            if(materialData->GetTexture(aiTextureType_DIFFUSE_ROUGHNESS, 0, &path) == aiReturn_SUCCESS)
            {
                std::string fullPath = directory + '/' + path.data;

                std::shared_ptr<Texture> diffuse = Texture::Load2D(fullPath);
                
                material.SetDiffuseSampler(Sampler::Create(diffuse));
            }

            if(materialData->GetTexture(aiTextureType_SPECULAR, 0, &path) == aiReturn_SUCCESS)
            {
                std::string fullPath = directory + '/' + path.data;

                std::shared_ptr<Texture> specular = Texture::Load2D(fullPath);
                
                material.SetSpecularSampler(Sampler::Create(specular));
            }

            if(materialData->GetTexture(aiTextureType_NORMALS, 0, &path) == aiReturn_SUCCESS)
            {
                std::string fullPath = directory + '/' + path.data;

                std::shared_ptr<Texture> normal = Texture::Load2D(fullPath);
                
                material.SetNormalSampler(Sampler::Create(normal));
            }

            if(materialData->GetTexture(aiTextureType_OPACITY, 0, &path) == aiReturn_SUCCESS)
            {
                std::string fullPath = directory + '/' + path.data;

                std::shared_ptr<Texture> opacity = Texture::Load2D(fullPath);
                
                material.SetOpacitySampler(Sampler::Create(opacity));
            }

            model.materials.push_back(std::move(material));
        }

        for(size_t i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* meshData = scene->mMeshes[i];

            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            Mesh mesh;

            for(size_t j = 0; j < meshData->mNumVertices; j++)
            {                
                Vertex vertex;

                aiVector3D& translation = meshData->mVertices[j];
                aiVector3D& normal = meshData->mNormals[j];

                aiVector3D uv = aiVector3D(0.0f);

                if(meshData->HasTextureCoords(0))
                {
                    uv = meshData->mTextureCoords[0][j]; 
                }

                aiColor4D color = aiColor4D(1.0f);

                if(meshData->HasVertexColors(0))
                {
                    color = meshData->mColors[0][j]; 
                }

                aiVector3D tangent = meshData->mTangents[j];
                
                vertex.Translation = glm::vec3(translation.x, translation.y, translation.z);
                vertex.Normal = glm::vec3(normal.x, normal.y, normal.z);
                vertex.Uv = glm::vec2(uv.x, uv.y);
                vertex.Color = glm::vec3(color.r, color.g, color.b);
                vertex.Tangent = glm::vec3(tangent.x, tangent.y, tangent.z);

                vertices.push_back(vertex);
            }

            for(size_t j = 0; j < meshData->mNumFaces; j++)
            {
                aiFace& face = meshData->mFaces[j];

                for(size_t k = 0; k < face.mNumIndices; k++)
                {
                    indices.push_back(face.mIndices[k]);
                }
            }

            mesh.SetVertices(vertices.data(), vertices.size());
            mesh.SetIndices(indices.data(), indices.size());    

            model.parts.push_back({ std::move(mesh), meshData->mMaterialIndex });
        }

        return model;
    }

    Model::Model()
    {

    }

    Model::~Model()
    {

    }

    Model::Model(Model&& other)
    {
        *this = std::move(other);
    }
    
    Model& Model::operator=(Model&& other)
    {
        parts = std::move(other.parts);
        materials = std::move(other.materials);

        return *this;
    }

    const std::vector<Model::Part>& Model::GetParts() const
    {
        return parts;
    }
    
    const std::vector<Material>& Model::GetMaterials() const
    {
        return materials;
    }
    
    std::vector<Model::Part>& Model::GetParts()
    {
        return parts;
    }

    std::vector<Material>& Model::GetMaterials()  
    {
        return materials;
    }

    size_t Model::GetPartCount() const
    {
        return parts.size(); 
    }

    size_t Model::GetMaterialCount() const
    {
        return materials.size();
    }
}