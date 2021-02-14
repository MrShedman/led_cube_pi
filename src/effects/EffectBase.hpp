#pragma once

#include "led-matrix.h"

#include "pixel-mapper.h"
#include "graphics.h"

#include "chrono/clock.h"

#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <algorithm>

using std::max;
using std::min;

using namespace rgb_matrix;

class EffectBase
{
protected:
    EffectBase(Canvas *canvas);
    inline Canvas *canvas() { return canvas_; }

public:
    virtual ~EffectBase() {}
    virtual void Init() = 0;
    virtual void OnFrame() = 0;
    void Run();

private:
    const Time timePerFrame = hertz(100.0f);
    Canvas *const canvas_;
};