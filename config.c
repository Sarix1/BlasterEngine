#include "config.h"
#include <SDL2/SDL_video.h>

int loadConfig(struct Config* config, char* filename)
{
    printf("loadConfig()\n");
    // Try to load configuration from file here; success returns; failure sets default values below

    config->title        = "Blasterline";
    config->windowWidth  = 320;
    config->windowHeight = 240;
    config->screenWidth  = 256;
    config->screenHeight = 224;
    config->windowFlags  = SDL_WINDOW_SHOWN;// | SDL_WINDOW_BORDERLESS;// | SDL_WINDOW_FULLSCREEN;
    config->resScale     = 2;

    return 0;
}
