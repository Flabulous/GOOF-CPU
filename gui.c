#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <SDL.h>
#include "goof.h"

int const SCALE = 24; //You can change this to anything you want depending on your personal screen size
int const SIZE = 12; //Amount of pixels of the screen
SDL_Window* gui_window = NULL;
SDL_Surface* gui_screen = NULL;
SDL_Rect gui_rects[144];

int initGUI()
{
    printf("Initializing GUI... ");
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("\nSDL Error that I don't know how to print!\n");
    } else {
        gui_window = SDL_CreateWindow("GOOF-GUI", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SIZE*SCALE, SIZE*SCALE, SDL_WINDOW_SHOWN);
        if(gui_window == NULL ) {
            printf("\nWindow could not be created!\n");
        } else {
            gui_screen = SDL_GetWindowSurface(gui_window);
            SDL_FillRect(gui_screen, NULL, SDL_MapRGB(gui_screen->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(gui_window);
        }
    }

    for (int i = SIZE; i >= 0; i--) {
        for (int j = SIZE; j >= 0; j--) {
            gui_rects[i+j].x = i*SCALE;
            gui_rects[i+j].y = j*SCALE;
            gui_rects[i+j].w = i+SCALE;
            gui_rects[i+j].h = j+SCALE;
        }
    }
    printf("GUI Initialized.\n");
    return 0;
}

int refreshGUI()
{
    SDL_FillRect(gui_screen, NULL, SDL_MapRGB(gui_screen->format, 0x00, 0x00, 0x00));
    SDL_PollEvent(&e);
    for (int x = 12; x >= 0; x--) {
        for (int y = 12; y >= 0; y--) {
            SDL_FillRect(gui_screen, &gui_rects[x+y], SDL_MapRGB(gui_screen->format, ram[x+y], ram[x+y], ram[x+y]));
        }
    }
    SDL_UpdateWindowSurface(gui_window);
    return 0;
}
