#pragma once

#include <string>
#include <cstdint>
#include <glm/glm.hpp>

#include "WindowState.h"
#include "../Event.h"
#include "../../Input/Key.h"
#include "../../Input/MouseButton.h"

namespace Quanta
{
    class Window
    {
    public:
        static std::shared_ptr<Window> Create();

        virtual ~Window() = 0;
        
        virtual void AddKeyDownCallback(Event<Key>::Handler handler) = 0;
        virtual void AddKeyUpCallback(Event<Key>::Handler handler) = 0;

        virtual void AddMouseDownCallback(Event<MouseButton>::Handler handler) = 0;
        virtual void AddMouseUpCallback(Event<MouseButton>::Handler handler) = 0;

        virtual void AddMouseMoveCallback(Event<glm::vec2>::Handler handler) = 0;
        virtual void AddMouseScrollCallback(Event<glm::vec2>::Handler handler) = 0;

        virtual void AddCharacterDownCallback(Event<char>::Handler handler) = 0;
        
        virtual WindowState GetState() const = 0;
        virtual void SetState(WindowState value) = 0;
        
        virtual glm::uvec2 GetPosition() const = 0;
        virtual glm::uvec2 GetSize() const = 0;
        
        virtual glm::uvec4 GetBounds() const = 0;

        virtual uint32_t GetX() const = 0;
        virtual void SetX(uint32_t value) = 0;

        virtual uint32_t GetY() const = 0;
        virtual void SetY(uint32_t value) = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual void SetWidth(uint32_t value) = 0;
        
        virtual uint32_t GetHeight() const = 0;
        virtual void SetHeight(uint32_t value) = 0;

        virtual bool Exists() const = 0;
        virtual void PollEvents() const = 0;
        virtual void SwapBuffers() const = 0;
    };
}