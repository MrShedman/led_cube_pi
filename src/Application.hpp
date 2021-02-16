#pragma once

#include "chrono/time.h"

#include "led-matrix.h"

#include "effects/EffectBase.hpp"

#include <map>

class Application
{
public:

	Application();

    bool init();

	void run();

	void close()
	{
        is_open = false;
	}

private:

	const Time timePerFrame = hertz(100.0f);

    bool is_open;
    rgb_matrix::RGBMatrix* matrix;
    int current_effect_id;
    std::map<int, EffectBase::Ptr> effects;
};