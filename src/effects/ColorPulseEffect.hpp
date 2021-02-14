#pragma once

#include "EffectBase.hpp"

// Simple generator that pulses through RGB and White.
class ColorPulseEffect : public EffectBase
{
public:
    ColorPulseEffect(RGBMatrix *m) : EffectBase(m), matrix_(m)
    {
        off_screen_canvas_ = m->CreateFrameCanvas();
    }

    void Init() override
    {
        continuum = 0;
    }

    void OnFrame() override
    {
        continuum += 1;
        continuum %= 3 * 255;
        int r = 0, g = 0, b = 0;
        if (continuum <= 255)
        {
            int c = continuum;
            b = 255 - c;
            r = c;
        }
        else if (continuum > 255 && continuum <= 511)
        {
            int c = continuum - 256;
            r = 255 - c;
            g = c;
        }
        else
        {
            int c = continuum - 512;
            g = 255 - c;
            b = c;
        }
        off_screen_canvas_->Fill(r, g, b);
        off_screen_canvas_ = matrix_->SwapOnVSync(off_screen_canvas_);
    }

private:
    uint32_t continuum;
    RGBMatrix *const matrix_;
    FrameCanvas *off_screen_canvas_;
};