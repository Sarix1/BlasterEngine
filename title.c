#include "title.h"
#include "state.h"
#include <stdio.h>
#include <stdlib.h> // malloc should work through a memory.c command instead

int initState_Title()  // should take message bus instead
{
    printf("initState_Title()\n");

    return 0;
}

int killState_Title()
{
    printf("killState_Title()\n");

    return 0;
}

int resumeState_Title()
{
    printf("resumeState_Title()\n");

    return 0;
}

int pauseState_Title()
{
    printf("pauseState_Title()\n");

    return 0;
}

int updateState_Title()
{
    printf("updateState_Title()\n");

    return 0;
}

int drawState_Title()
{
    printf("drawState_Title()\n");

    return 0;
}

struct State* createState_Title()
{
    struct State* State = malloc(sizeof(struct State));

    State->enable_update    = 1;
    State->enable_draw      = 1;
    State->Data             = (struct TitleData*)malloc(sizeof(struct TitleData));
    State->init             = initState_Title;
    State->kill             = killState_Title;
    State->resume           = resumeState_Title;
    State->pause            = pauseState_Title;
    State->update           = updateState_Title;
    State->draw             = drawState_Title;

    return State;
}
