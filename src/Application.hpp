#pragma once

#include "chrono/time.h"

class Application
{
public:

	Application();

	void run();

	void close()
	{
	}

private:

	const Time timePerFrame = seconds(1.0f / 60.0f);

	void render();
};