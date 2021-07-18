#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

struct Config
{
    char* title;
    char* font;
    int windowWidth, windowHeight;
    int screenWidth, screenHeight;
    uint32_t windowFlags;
    int resScale;
};

int loadConfig(struct Config* config, char* filename);

#endif

