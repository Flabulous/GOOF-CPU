#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL.h>
#include "goof.h"

int SCALE = 24; //You can change this to anything you want depending on your personal screen size
SDL_Window* gui_window = NULL;
SDL_Surface* gui_screen = NULL;

int initGUI()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Error that I don't know how to print!");
    } else {
        gui_window = SDL_CreateWindow("GOOF-GUI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 12*SCALE, 12*SCALE, SDL_WINDOW_SHOWN);
        if(gui_window == NULL ) {
            printf("Window could not be created!");
        } else {
            gui_screen = SDL_GetWindowSurface(gui_window);
            SDL_FillRect(gui_screen, NULL, SDL_MapRGB(gui_screen->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(gui_window);
        }
    }
    return 0;
}

int refreshGUI()
{

    return 0;
}
