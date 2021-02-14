#include "Utilities.hpp"

#include <iostream>
#include <limits>

#include <random>

namespace Util
{
float randomize(float min, float max)
{
	std::random_device rd;
	std::default_random_engine generator(rd());
	std::uniform_real_distribution<float> distribution(min, max);
	return distribution(generator);
}

bool isKeyPressed(SDL_Scancode key)
{
    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);

    return currentKeyStates[key];
}
}