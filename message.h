#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_MESSAGES 256

struct Message
{
    void* p;
};

struct MessageBus
{
    struct Message InputQueue [MAX_MESSAGES];
    struct Message OutputQueue[MAX_MESSAGES];
    struct Message VideoQueue [MAX_MESSAGES];
    struct Message AudioQueue [MAX_MESSAGES];
};

#endif
