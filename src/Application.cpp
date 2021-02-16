
#include "Application.hpp"
#include "Utilities.hpp"

#include "chrono/clock.h"

#include "ConfigParser.hpp"
#include "resource/ResourcePaths.hpp"
#include "effects/ColorPulseEffect.hpp"

#include <string>
#include <iostream>
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


Application::Application()
{

}

bool Application::init()
{
    initHandlers();

    ConfigParser configParser;
    configParser.parseYaml(getResourcePath() + "config.yaml");

    rgb_matrix::RGBMatrix::Options matrix_options = configParser.getMatrixOptions();
    rgb_matrix::RuntimeOptions runtime_opt = configParser.getRuntimeOptions();

    matrix = rgb_matrix::RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    if (matrix == NULL)
        return false;

    printf("Size: %dx%d. Hardware gpio mapping: %s\n",
          matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    current_effect_id = 0;
    effects[0] = std::shared_ptr<EffectBase>(new ColorPulseEffect(matrix));

    is_open = true;
    return true;
}

void Application::run()
{
    uint64_t frame_id = 0;
    Clock clock;
    while (!interrupt_received && is_open)
    {
        //effects[current_effect_id]->OnFrame(timePerFrame);
        auto dt = clock.restart();
        sleep(timePerFrame - dt);
        frame_id++;
        std::cout << "Frame: " << frame_id << std::endl;
    }

    std::cout << "[Application] Exiting...." << std::endl;
}