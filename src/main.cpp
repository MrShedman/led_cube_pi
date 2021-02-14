
#include "led-matrix.h"

#include "ConfigParser.hpp"
#include "resource/ResourcePaths.hpp"

#include "effects/ColorPulseEffect.hpp"

int main(int argc, char *argv[])
{
    ConfigParser configParser;
    configParser.parseYaml(getResourcePath() + "config.yaml");

    rgb_matrix::RGBMatrix::Options matrix_options = configParser.getMatrixOptions();
    rgb_matrix::RuntimeOptions runtime_opt = configParser.getRuntimeOptions();

    rgb_matrix::RGBMatrix *matrix = rgb_matrix::RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    if (matrix == NULL)
        return 1;

    printf("Size: %dx%d. Hardware gpio mapping: %s\n",
           matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    rgb_matrix::Canvas *canvas = matrix;

    ColorPulseEffect effect(matrix);

    effect.Run();

    return 0;
}