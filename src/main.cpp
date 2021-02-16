
#include "Application.hpp"

int main(int argc, char *argv[])
{
    Application app;

    if (app.init())
    {
        app.run();
    }
    
    return 0;
}