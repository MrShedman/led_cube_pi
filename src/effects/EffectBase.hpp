#pragma once

#include "led-matrix.h"

#include "pixel-mapper.h"
#include "graphics.h"

#include "chrono/time.h"

#include <memory>

class EffectBase
{
protected:
    EffectBase(rgb_matrix::Canvas *canvas) : canvas_(canvas) {}
    inline rgb_matrix::Canvas *canvas() { return canvas_; }

public:
    typedef std::shared_ptr<EffectBase> Ptr;

    virtual ~EffectBase() {}
    virtual void Init() = 0;
    virtual void OnFrame(const Time &dt) = 0;

private:
    rgb_matrix::Canvas *const canvas_;
};