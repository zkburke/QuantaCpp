#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Mesh.h" 
#include "Material.h"

namespace Quanta
{
    class Model final
    {
    public:
        struct Part
        {
            Mesh mesh;
            glm::mat4 transform = glm::mat4(1.0f);

            size_t materialIndex = 0;
        };

        static Model FromFile(const std::string& filepath);

        Model();
        ~Model();

        Model(Model&&);
        
        Model& operator=(Model&&);

        Model(const Model&) = delete;

        Model& operator=(const Model&) = delete;
        
        const std::vector<Part>& GetParts() const;
        const std::vector<Material>& GetMaterials() const; 

        std::vector<Part>& GetParts();
        std::vector<Material>& GetMaterials(); 

        size_t GetPartCount() const;
        size_t GetMaterialCount() const;
    private:
        std::vector<Part> parts;
        std::vector<Material> materials;
    };
}