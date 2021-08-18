#pragma once

#include <Quanta/Graphics/Shader/ShaderModule.h>

namespace Quanta::OpenGL
{
    class ShaderModule final : public Quanta::ShaderModule
    {
    public:
        ShaderModule(ShaderType type, const std::string& source);
        ~ShaderModule();

        ShaderModule(const ShaderModule&) = delete;

        ShaderModule& operator=(const ShaderModule&) = delete;
        
        UInt32 GetHandle() const;
    private:
        UInt32 handle;
    };
}