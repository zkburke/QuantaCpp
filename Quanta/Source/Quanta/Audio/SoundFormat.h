#pragma once

#include <stdint.h>

namespace Quanta
{
    enum class SoundFormat : uint8_t
    {
        Mono8,
        Mono16,
        Stereo8,
        Stereo16,
        TwoDimentional16,
        ThreeDimentional16
    };
}