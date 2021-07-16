#ifndef VIDEO_H
#define VIDEO_H

#include <SDL2/SDL.h>
#include <stdint.h>
#include "config.h"
#include "message.h"

struct Font
{
    SDL_Texture* Texture;
    SDL_Surface* Surface;
    int width;
    int height;
};

struct Graphics
{
    struct Font BasicFont;
};

struct Video
{
    int                 windowWidth, windowHeight;
    uint32_t            windowFlags;
    SDL_Window*         Window;
    SDL_Renderer*       Renderer;
    SDL_Rect            Screen;
    struct Graphics     Graphics;
};

int initVideo           (struct Video* video, struct Config* config);
int killVideo           (struct Video* video);
int execVideo           (struct Video* video);
int execGraphics        (struct Video* video, struct MessageBus* MessageBus);
int initFont            (struct Font* font, const char* filename, int w, int h, SDL_Renderer* renderer);
int drawText            (const char* text, struct Font* font, int x, int y, int max_columns, int max_rows, uint32_t color, struct Video* video);
int drawTextCentered    (const char* text, struct Font* font, uint32_t color, struct Video* video);
uint32_t colorToUint    (int r, int g, int b);
SDL_Color uintToColor   (uint32_t color);

#endif
