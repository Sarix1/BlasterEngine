#include "system.h"

int main(int argc, char* argv[])
{
    struct System System;

    initSystem(&System, argv[1]);

    while (System.running)
        execSystem(&System);

    quitSystem(&System);

    return 0;
}
