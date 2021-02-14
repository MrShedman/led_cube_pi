#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <bitset>

#include <SDL.h>

namespace Util
{
    template<typename T>
    inline T Clamp(const T &a, const T &min, const T &max)
    {
        if (a < min) return min;
        else if (a > max) return max;
        else return a;
    }

    template<typename T>
    std::string printBits(T number)
    {
        return std::bitset<sizeof(T) * 8>(number).to_string();
    }

    float randomize(float min, float max);
    
    bool isKeyPressed(SDL_Scancode key);
}