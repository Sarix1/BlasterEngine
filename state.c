#include "state.h"
#include "title.h"
#include <stdio.h>

// updatestate must received System* as an argument to access global data
// system has a pointer StateManager*, which updatestate may use to change states
// input is handled only to the current state
// based on the input and updated logic, the state generates commands

int pushState(struct State* state, struct StateList* stateList)
{
    stateList->States[stateList->numStates++] = state;

    return 0;
}

int popState(struct State* state, struct StateList* stateList)
{
    stateList->States[--stateList->numStates] = NULL;

    return 0;
}

int initStateManager(struct StateManager* stateMgr)
{
    printf("initStateManager()\n");

    stateMgr->CurrentState          = NULL;
    stateMgr->StateList.numStates   = 0;

    struct State* Title = createState_Title();
    pushState(Title, &stateMgr->StateList);

    return 0;
}

int updateAllStates(struct StateManager* stateMgr, double dt)
{
    struct State** States = stateMgr->StateList.States;
    int numStates        = stateMgr->StateList.numStates;
    int i;

    if (numStates == 0)
        printf("updateStates(): Nothing to update\n");
    else
    {
        for (i = 0; i < numStates; i++)
        {
            if (States[i]->enable_update)
                States[i]->update(0.0);
        }
    }

    return 0;
}

int switchState(struct StateManager* stateMgr, struct State* state)
{
    state->LastState = stateMgr->CurrentState;
    stateMgr->CurrentState = state;

    if ((state->type & STATE_PAUSE_LAST_UPDATE) || !(state->LastState->type & STATE_BACKGROUND_UPDATE))
        state->LastState->enable_update = 0;

    if ((state->type & STATE_PAUSE_LAST_DRAW) || !(state->LastState->type & STATE_BACKGROUND_DRAW))
        state->LastState->enable_draw = 0;

    return 0;
}
