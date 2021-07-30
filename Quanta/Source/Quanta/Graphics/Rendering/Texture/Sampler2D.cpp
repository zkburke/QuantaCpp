#include "Sampler2D.h"
#include "../GraphicsDevice.h"
#include "../../../../Platform/Rendering/OpenGL/OpenGLSampler2D.h"

namespace Quanta
{
    std::shared_ptr<Sampler2D> Sampler2D::Create(const std::shared_ptr<Texture2D>& texture) 
    {
        switch(GraphicsDevice::GetApi())
        {
        case GraphicsApi::OpenGL:
            return std::make_unique<OpenGLSampler2D>(texture);            
        }
        
        return nullptr;
    }
}