#include "message.h"
#include <stdio.h>

int initMessageBus(struct MessageBus* messageBus)
{
    printf("initMessageBus()\n");

    messageBus->head = 0;
    messageBus->tail = 0;
    messageBus->numMessages = 0;

    return 0;
}

int pushMessage(struct MessageBus* messageBus, struct Message message)
{
    if (messageBus->numMessages < MAX_MESSAGES)
    {
        messageBus->Messages[messageBus->tail++] = message;

        if (messageBus->tail >= MAX_MESSAGES)
            messageBus->tail = 0;
    }
    else
        return 1;

    return 0;
}

int pollMessages(struct MessageBus* messageBus)
{
    while (messageBus->numMessages-- > 0)
    {
        //processMessage(&messageBus->Messages[messageBus->head++]);

        if (messageBus->head >= MAX_MESSAGES)
            messageBus->head = 0;
    }

    return 0;
}

