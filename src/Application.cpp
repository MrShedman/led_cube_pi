
#include "Application.hpp"
#include "Utilities.hpp"

#include "chrono/clock.h"

#include "ConfigParser.hpp"
#include "resource/ResourcePaths.hpp"
#include "effects/ColorPulseEffect.hpp"

#include <string>
#include <iostream>
#include <signal.h>

#include "mpu9250.hpp"
#include <glm/gtx/io.hpp>

#define BLYNK_PRINT stdout
#ifdef RASPBERRY
  #include <BlynkApiWiringPi.h>
#else
  #include <BlynkApiLinux.h>
#endif
#include <BlynkSocket.h>
#include <BlynkOptionsParser.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);

static const char *auth = "0f8Meh0pQ71skG-NRMDlprEmh8KgRCTq";
static const char *serv = "localhost";
static uint16_t port = 8080;

#include <BlynkWidgets.h>

BlynkTimer tmr;

BLYNK_WRITE(V1)
{
    printf("Got a value: %s\n", param[0].asStr());
}


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

    // rgb_matrix::RGBMatrix::Options matrix_options = configParser.getMatrixOptions();
    // rgb_matrix::RuntimeOptions runtime_opt = configParser.getRuntimeOptions();

    // matrix = rgb_matrix::RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    // if (matrix == NULL)
    //     return false;

    // printf("Size: %dx%d. Hardware gpio mapping: %s\n",
    //       matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    // current_effect_id = 0;
    // effects[0] = std::shared_ptr<EffectBase>(new ColorPulseEffect(matrix));

    i2c = new I2C(configParser.get_imu_dev(), configParser.get_imu_address());
    mpu9250 = new MPU9250(i2c, configParser.get_imu_address());

    Blynk.begin(auth, serv, port);
    tmr.setInterval(1000, [](){
      Blynk.virtualWrite(V0, BlynkMillis()/1000);
    });

    is_open = true;
    return true;
}

void Application::run()
{
    uint64_t frame_id = 0;
    Clock clock;
    while (!interrupt_received && is_open)
    {
	Blynk.run();
    	tmr.run();

        //effects[current_effect_id]->OnFrame(timePerFrame);
        auto dt = clock.restart();
        sleep(timePerFrame - dt);
        frame_id++;
        mpu9250->Read();
//        std::cout << mpu9250->gyro_radps() << ", " << mpu9250->accel_mps2() << ", " << mpu9250->die_temperature_c() << std::endl;
        //std::cout << "Frame: " << frame_id << std::endl;
    }

    std::cout << "[Application] Exiting...." << std::endl;
}
