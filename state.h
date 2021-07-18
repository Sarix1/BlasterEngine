#ifndef STATE_H
#define STATE_H

#define MAX_STATES 10

enum StateTypes
{
    STATE_RESET_ON_ENTRY      = 1 << 0,   // Reset on entry, e.g. a menu always starts from the top
    STATE_KILL_ON_EXIT        = 1 << 1,   // Call kill: deallocate memory and destroy upon exit, e.g. singleplayer game, but not console
    STATE_BACKGROUND_UPDATE   = 1 << 2,   // Update even when not current state, e.g. multiplayer game behind console
    STATE_BACKGROUND_DRAW     = 1 << 3,   // Draw even when not current state, e.g. singleplayer game behind pause menu
    STATE_PAUSE_LAST_UPDATE   = 1 << 4,   // overrides STATE_BACKGROUND_UPDATE, e.g. pause menu for singleplayer does this, but console won't
    STATE_PAUSE_LAST_DRAW     = 1 << 5    // overrides STATE_BACKGROUND_DRAW
};

struct State
{
    int type;
    int enable_update;
    int enable_draw;
    void* Data;

    int (*init)  ();
    int (*kill)  ();
    int (*resume)();
    int (*pause) ();
    int (*update)(double);
    int (*draw)  (double);

    struct State* LastState;
};

struct StateList
{
    struct State* States[MAX_STATES];
    int numStates;  // First In Last Out
};

struct StateManager
{
    struct StateList StateList;
    struct State* CurrentState;
};

int pushState       (struct State* state, struct StateList* stateList);
int popState        (struct State* state, struct StateList* stateList);
int initStateManager(struct StateManager* stateMgr);
int updateAllStates (struct StateManager* stateMgr, double dt);
int switchState     (struct StateManager* stateMgr, struct State* state);

#endif
