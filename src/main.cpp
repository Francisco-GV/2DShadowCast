#include "canvas.h"
#include "config.h"

int main(int argc, char** argv)
{
    config::loadConfigFile(argv[0]);

    Canvas canvas;
    canvas.start();
}
