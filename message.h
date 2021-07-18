#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

#define MAX_MESSAGES 4096

enum MessageTypes
{
    MSG_SWITCH_STATE,
    MSG_QUIT
};

struct Rect
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
};

union Data
{
    void*       Pointer;
    char*       String;
    uint8_t     Bytes[8];
    uint32_t    Uint32[2];
    uint64_t    Uint64;
    float       Float[2];
    double      Double;
    struct Rect Rect;
}; // 8 bytes

struct Message
{
    uint16_t    type;
    uint16_t    flags;
    uint32_t    timestamp;
    union Data  Payload;
}; // 16 bytes

struct MessageBus
{
    int head, tail, numMessages;
    struct Message Messages[MAX_MESSAGES];
};

int initMessageBus  (struct MessageBus* messageBus);
int pushMessage     (struct MessageBus* messageBus, struct Message message);
int pollMessages    (struct MessageBus* messageBus);
int processMessage  (struct Message* message);

#endif
