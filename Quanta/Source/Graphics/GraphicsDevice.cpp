#include <Quanta/Graphics/GraphicsDevice.h>

#include "../Platform/OpenGL/OpenGLVertexArray.h"
#include "../Platform/OpenGL/OpenGLGraphicsBuffer.h"
#include "../Platform/OpenGL/OpenGLGraphicsDevice.h"

namespace Quanta
{
    struct State
    {
        GraphicsDevice* device = nullptr;
        GraphicsApi api = GraphicsApi::OpenGL;
    } static state;
    
    GraphicsApi GraphicsDevice::GetApi()
    {
        return state.api;
    }

    void GraphicsDevice::Initialize(GraphicsApi api)
    {
        state.api = api;

        switch(api)
        {
        case GraphicsApi::OpenGL:
            state.device = new OpenGLGraphicsDevice();

            break;
        }
    }
    
    void GraphicsDevice::DeInitialize()
    {
        delete state.device;
    }

    void GraphicsDevice::ClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        state.device->InternalClearBackBuffer(color, depth, stencil);
    }
        
    void GraphicsDevice::SetRasterPipeline(const std::shared_ptr<RasterPipeline>& value)
    {
        state.device->InternalSetRasterPipeline(value);
    }

    void GraphicsDevice::SetVertexArray(const std::shared_ptr<VertexArray>& value)
    {
        state.device->InternalSetVertexArray(value);
    }
    
    void GraphicsDevice::BindSampler(const Sampler2D& sampler, size_t index)
    {
        state.device->InternalBindSampler(sampler, index);
    }
    
    void GraphicsDevice::BindSampler(const Sampler3D& sampler, size_t index)
    {
        state.device->InternalBindSampler(sampler, index);
    }
    
    void GraphicsDevice::BindSampler(const SamplerCube& sampler, size_t index)
    {
        state.device->InternalBindSampler(sampler, index);
    }

    void GraphicsDevice::DispatchDraw(const DrawCommand& command)
    {
        state.device->InternalDispatchDraw(command);
    }
}