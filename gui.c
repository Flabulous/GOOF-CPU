#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL.h>

int SCALE = 4; //You can change this to anything you want depending on your personal screen size
SDL_Window* gui_window = NULL;
SDL_Surface* gui_screen = NULL;

int initSDLGUI()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError);
    }
    return 0;
}
