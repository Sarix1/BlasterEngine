#include "input.h"
#include <string.h>
#include <stdio.h>

int initInput(struct Input* input)
{
    printf("initInput()\n");
    input->TextField = NULL;

    return 0;
}

int pollEvents(struct Input* input)
{
    SDL_Event* event = &input->Event;
    char* buffer     = input->TextField->buffer;
    int* cursor      = &input->TextField->cursor;
    int key;

    while (SDL_PollEvent(event))
    {
        if (event->type == SDL_QUIT)
        {
            return 1;
        }
        else if (event->type == SDL_KEYDOWN)
        {
            key = event->key.keysym.sym;

            if (key == SDLK_ESCAPE)
                return 1;

            else if (key == SDLK_RETURN)
            {
                buffer[(*cursor)++] = '\n';
                buffer[*cursor] = '\0';
            }
            else if (key == SDLK_BACKSPACE && *cursor > 0)
                buffer[--(*cursor)] = '\0';

            else if (event->key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                SDL_SetClipboardText(buffer);

            else if (event->key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
            {
                strncpy(buffer, SDL_GetClipboardText(), MAX_INPUT);
                *cursor = strlen(buffer);
            }
        }
        else if (event->type == SDL_TEXTINPUT)
        {
            if (!(SDL_GetModState() & KMOD_CTRL && (event->text.text[0] == 'c' ||
                                                    event->text.text[0] == 'C' ||
                                                    event->text.text[0] == 'v' ||
                                                    event->text.text[0] == 'V')))
            {
                if (*cursor < MAX_INPUT)
                {
                    buffer[(*cursor)++] = event->text.text[0];
                    buffer[*cursor] = '\0';
                }
            }
        }
    }

    return 0;
}

int execInput(struct Input* input)
{
    /*
    if a message with a memory pointer is found, set textbuffer to it
    struct TitleData* TitleData = (struct TitleData*)&Title->Data;
    system->Input.TextField = &TitleData->TextField;
    */
    int ret_val;

    ret_val = pollEvents(input);
    input->Keys = SDL_GetKeyboardState(NULL);

    // buffer should get pushed onto message queue after that if you pressed Enter, and call initTextField again

    return ret_val;
}

int initTextField(struct TextField* textField)
{
    textField->cursor = 0;
    textField->buffer[0] = '\0';
}
