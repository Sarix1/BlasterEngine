#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
//#include "config.h"

#define MAX_INPUT 512

struct TextField
{
    char buffer[MAX_INPUT];
    int cursor;
};

struct Input
{
    SDL_Event Event;
    uint8_t* Keys;
    struct TextField* TextField;
};

int initInput(struct Input* input);// include later for key configs etc , struct Config* config);
int killInput(struct Input* input);
int execInput(struct Input* input);

#endif


