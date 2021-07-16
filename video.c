#include "video.h"
#include "text.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int initVideo(struct Video* video, struct Config* config) // could be split into functions, including initgraphics
{
    printf("initVideo()\n");

    // SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Error - SDL_Init(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());

        return 1;
    }

    // Window
    video->windowWidth  = config->windowWidth;
    video->windowHeight = config->windowHeight;
    video->windowFlags  = config->windowFlags;
    video->Window = SDL_CreateWindow
    (
        config->title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        video->windowWidth * config->resScale,
        video->windowHeight * config->resScale,
        video->windowFlags
    );

    if (video->Window == NULL)
    {
        SDL_Log("Error - Failed to create SDL_Window: %s\n", SDL_GetError());

        return 1;
    }

    // Renderer
    video->Renderer = SDL_CreateRenderer(video->Window, -1, 0);

    if (video->Renderer == NULL)
    {
        SDL_Log("Error: Failed to create SDL_Renderer: %s\n", SDL_GetError());

        return 1;
    }

    if (config->resScale > 1)
        SDL_RenderSetScale(video->Renderer,config->resScale, config->resScale);

    if (config->screenWidth < config->windowWidth || config->screenHeight < config->windowHeight)
    {
        video->Screen.x = (video->windowWidth - config->screenWidth)/2;
        video->Screen.y = (video->windowHeight - config->screenHeight)/2;
        video->Screen.w = config->screenWidth;
        video->Screen.h = config->screenHeight;
        SDL_RenderSetViewport(video->Renderer, &video->Screen);
    }

    // Graphics
    if (initFont(&video->Graphics.BasicFont, "font.bmp", 8, 8, video->Renderer) == 1)
    {
        printf("Error - initFont() failed\n");

        return 1;
    }

    return 0;
}

int killVideo(struct Video* video)
{
    SDL_DestroyWindow(video->Window);

    return 0;
}

int execVideo(struct Video* video)
{
    printf("execVideo()\n");
    execGraphics(video, NULL);
    SDL_RenderPresent(video->Renderer);

    return 0;
}

int execGraphics(struct Video* video, struct MessageBus* MessageBus)
{
    // Blank screen
    SDL_SetRenderDrawColor(video->Renderer, 64, 128, 255, 255);
    SDL_RenderFillRect(video->Renderer, NULL);

    // Walk through message bus here to render everything

    // temporary test functions
    const char* text = "VIDEO TEST";
    drawTextCentered(text, &video->Graphics.BasicFont, 0xFFFFFFFF, video);

    return 0;
}

int initFont(struct Font* font, const char* filename, int w, int h, SDL_Renderer* renderer)
{
    printf("initFont()\n");

    font->Surface   = SDL_LoadBMP(filename);
    SDL_SetColorKey(font->Surface, SDL_TRUE, 0x00000000);
    font->width     = w;
    font->height    = h;

    if (font->Surface == NULL)
    {
        printf("Error - SDL_LoadBMP() returned NULL\n");

        return 1;
    }

    if ((font->Texture = SDL_CreateTextureFromSurface(renderer, font->Surface)) == NULL)
    {
        printf("Error - SDL_CreateTextureFromSurface() failed\n");

        return 1;
    }

    return 0;
}

int drawText(const char* text, struct Font* font, int x, int y, int max_columns, int max_rows, uint32_t color, struct Video* video)
{
    char* c;
    int row;
    int column;
    SDL_Rect source = {0, 0, font->width, font->height};
    SDL_Rect destination;

    // Do this with texture color modulation instead, get rid of SDL_Surface in struct Font
    static uint32_t last_color = 0x00000000;
    if (color != last_color)
    {
        font->Surface->format->palette->colors[1] = uintToColor(color);
        font->Texture = SDL_CreateTextureFromSurface(video->Renderer, font->Surface);
    }

    if (x >= 0)             x           *= font->width;
    if (y >= 0)             y           *= font->height;
    if (max_rows == 0)      max_rows     = (video->Screen.w-x) / font->height;
    if (max_columns == 0)   max_columns  = (video->Screen.h-y) / font->width;

    destination.x = x;
    destination.y = y;
    destination.w = font->width;
    destination.h = font->height;

    for (row = 0, c = text; row < max_rows; row++)
    {
        destination.x = x;

        for (column = 0; column < max_columns; column++, c++)
        {
            if (*c == '\0')
                return 0;
            else if (*c == '\n')
                column = max_columns;
            else
            {
                source.y = ((*c) / 32) * font->height;
                source.x = ((*c) % 32) * font->width;
                SDL_RenderCopy(video->Renderer, font->Texture, &source, &destination);
            }
            destination.x += font->width;
        }
        destination.y += font->height;
    }

    return 0;
}

int drawTextCentered(const char* text, struct Font* font, uint32_t color, struct Video* video)
{
    static int x = -1;
    static int y;

    if (x == -1)
        getTextPositionCentered(text, font->width, font->height, video->Screen.w, video->Screen.h, &x, &y);

    drawText(text, &video->Graphics.BasicFont, x, y, 0, 0, color, video);
}

uint32_t colorToUint(int r, int g, int b)
{
	return (uint32_t)((r << 16) + (g << 8) + (b << 0));
}

SDL_Color uintToColor(uint32_t color)
{
	SDL_Color tempcol;
	tempcol.a = 255;
	tempcol.r = (color >> 16) & 0xFF;
	tempcol.g = (color >> 8)  & 0xFF;
	tempcol.b =  color        & 0xFF;

	return tempcol;
}
