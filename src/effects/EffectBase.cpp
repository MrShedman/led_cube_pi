#include "EffectBase.hpp"

#include <signal.h>

namespace
{
    volatile bool interrupt_received = false;
    static void InterruptHandler(int signo)
    {
        interrupt_received = true;
    }

    bool interruptHandlersInit = false;

    void initHandlers()
    {
        if (!interruptHandlersInit)
        {
            signal(SIGTERM, InterruptHandler);
            signal(SIGINT, InterruptHandler);
            interruptHandlersInit = true;
        }
    }
} // namespace

EffectBase::EffectBase(rgb_matrix::Canvas *canvas)
    : canvas_(canvas)
{
    initHandlers();
}

void EffectBase::Run()
{
    Init();

    Clock clock;
    while (!interrupt_received)
    {
        OnFrame();
        auto dt = clock.get_elapsed_time();
        sleep(timePerFrame - dt);
    }
}
