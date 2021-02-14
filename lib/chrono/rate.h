#pragma once

#include "clock.h"

class Rate
{
public:

    Rate(const double hz);

    void sleep();

private:

    const Time m_loop_time;
    Clock m_loop_clock;
};