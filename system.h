#ifndef SYSTEM_H
#define SYSTEM_H

#include "config.h"
#include "state.h"
#include "message.h"
#include "video.h"
#include "input.h"

struct System
{
    int running;
    //struct Memory       Memory;
    struct Config       Config;
    struct StateManager StateManager;
    struct MessageBus   MessageBus;
    struct Video        Video;
    struct Input        Input;
};

int initSystem(struct System* system, char* filename);
int quitSystem(struct System* system);
int execSystem(struct System* system);

#endif
