#include "system.h"
#include "title.h"
#include <stdio.h>

int initSystem(struct System* system, char* filename)
{
    printf("initSystem()\n");

    int error = 0;
    system->running = 1;

  //error |= initMemory         (&system->Memory);
    error |= loadConfig         (&system->Config, filename);
    error |= initStateManager   (&system->StateManager);
    error |= initInput          (&system->Input);
    error |= initMessageBus     (&system->MessageBus);
    error |= initVideo          (&system->Video, &system->Config);
  //error |= initAudio          (&system->Audio);

    if (error)
        printf("Error - initSystem() failed\n");

    return error;
}

int execSystem(struct System* system)
{
    printf("execSystem()\n");

    system->running = execInput(&system->Input)^1;
    updateAllStates (&system->StateManager, 0.0); // uses input & state data to change variables & generate update commands
  //drawAllStates   (&system->StateManager, 0.0); // walks through state data & update commands in message bus to generate draw commands
  //soundAllStates  (&system->StateManager, 0.0); // walks through state data & update commands in message bus to generate sound commands
    execVideo       (&system->Video); // process video // walks through draw commands to draw the screen image and render it
  //execAudio       (&system->Audio); // process audio // walks through sound commands to make noises
  //memory allocator does occasional de-fragmentation of memory

    return 1;
}

int quitSystem(struct System* system)
{
    printf("quitSystem()\n");
    // kill all states and subsystems

    return 0;
}
