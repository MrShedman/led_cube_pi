
#include "Application.hpp"

#include <getopt.h>

#include "led-matrix.h"

using namespace rgb_matrix;

#define TERM_ERR "\033[1;31m"
#define TERM_NORM "\033[0m"

#include "effects/ColorPulseEffect.hpp"

int main(int argc, char *argv[])
{
    int demo = -1;
    int scroll_ms = 30;

    const char *demo_parameter = NULL;
    RGBMatrix::Options matrix_options;
    rgb_matrix::RuntimeOptions runtime_opt;

    // These are the defaults when no command-line flags are given.
    matrix_options.rows = 32;
    matrix_options.chain_length = 1;
    matrix_options.parallel = 1;

    // First things first: extract the command line flags that contain
    // relevant matrix options.
    if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_opt))
    {
        //return usage(argv[0]);
    }

    int opt;
    while ((opt = getopt(argc, argv, "dD:r:P:c:p:b:m:LR:")) != -1)
    {
        switch (opt)
        {
        case 'D':
            demo = atoi(optarg);
            break;

        case 'm':
            scroll_ms = atoi(optarg);
            break;

        default: /* '?' */
            break;//return usage(argv[0]);
        }
    }

    if (optind < argc)
    {
        demo_parameter = argv[optind];
    }

    if (demo < 0)
    {
        fprintf(stderr, TERM_ERR "Expected required option -D <demo>\n" TERM_NORM);
        //return usage(argv[0]);
    }

    RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_opt);
    if (matrix == NULL)
        return 1;

    printf("Size: %dx%d. Hardware gpio mapping: %s\n",
           matrix->width(), matrix->height(), matrix_options.hardware_mapping);

    Canvas *canvas = matrix;

    ColorPulseEffect effect(matrix);

    effect.Run();

    return 0;
}