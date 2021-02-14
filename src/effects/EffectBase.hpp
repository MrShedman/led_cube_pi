#pragma once

#include "led-matrix.h"

#include "pixel-mapper.h"
#include "graphics.h"

#include "chrono/clock.h"

class EffectBase
{
protected:
    EffectBase(rgb_matrix::Canvas *canvas);
    inline rgb_matrix::Canvas *canvas() { return canvas_; }

public:
    virtual ~EffectBase() {}
    virtual void Init() = 0;
    virtual void OnFrame() = 0;
    void Run();

private:
    const Time timePerFrame = hertz(100.0f);
    rgb_matrix::Canvas *const canvas_;
};