#pragma once

#include <memory>
#include <string>

#include "TextureType.h"
#include "../Imaging/Image32.h"

namespace Quanta
{
    class Texture
    {
    public:
        static std::shared_ptr<Texture> Create(TextureType type, size_t width, size_t height, size_t depth);

        static std::shared_ptr<Texture> Load2D(const std::string& filepath);

        static std::shared_ptr<Texture> FromImage2D(const Image32& image);

        virtual ~Texture() = 0;

        virtual void SetData(const void* data) = 0;
        virtual void SetData(const void* data, size_t xOffset, size_t yOffset, size_t zOffset) = 0;

        virtual TextureType GetType() const = 0;

        virtual size_t GetWidth() const = 0;
        virtual size_t GetHeight() const = 0;
        virtual size_t GetDepth() const = 0;
    };
}