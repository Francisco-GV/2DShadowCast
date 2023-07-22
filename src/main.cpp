#include "canvas.h"
#include "config.h"

int main()
{
    config::loadConfigFile();

    Canvas canvas;
    canvas.start();
}
