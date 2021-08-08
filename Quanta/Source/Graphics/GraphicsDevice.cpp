#include <Quanta/Graphics/GraphicsDevice.h>

#include "../Platform/OpenGL/OpenGLGraphicsDevice.h"
#include "../Debugging/Validation.h"

namespace Quanta
{
    struct State
    {
        GraphicsDevice* device = nullptr;
        GraphicsApi api = GraphicsApi::OpenGL;
    } static state;
    
    void GraphicsDevice::Initialize(const Window* window)
    {
        GraphicsApi api = window->GetGraphicsApi();

        DEBUG_ASSERT(api == GraphicsApi::OpenGL);

        switch(api)
        {
        case GraphicsApi::OpenGL:
            state.device = new OpenGLGraphicsDevice(window);

            break;
        }

        state.api = api;
    }
    
    void GraphicsDevice::DeInitialize()
    {
        delete state.device;
    }

    void GraphicsDevice::ClearBackBuffer(const glm::vec4& color, float depth, int stencil)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalClearBackBuffer(color, depth, stencil);
    }
        
    void GraphicsDevice::SetRasterPipeline(const RasterPipeline* value)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalSetRasterPipeline(value);
    }

    void GraphicsDevice::SetVertexArray(const VertexArray* value)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalSetVertexArray(value);
    }
        
    void GraphicsDevice::BindSampler(const Sampler* sampler, size_t index)
    {
        DEBUG_ASSERT(state.device != nullptr);

        state.device->InternalBindSampler(sampler, index);
    }
    
    void GraphicsDevice::DispatchDraw(const DrawCommand& command)
    {
        DEBUG_ASSERT(state.device != nullptr);
    
        state.device->InternalDispatchDraw(command);
    }
    
    GraphicsApi GraphicsDevice::GetApi()
    {
        return state.api;
    }
}