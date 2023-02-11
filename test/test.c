#include <stdio.h>
#include "../src/selection_to_pixel.h"
#include "../src/tools.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

char *PATH_IMAGE = "ressources/image_test_1.jpeg";
int square1[2] = {20,10};
int square2[2] = {40,20};
int main()
{
    SDL_Surface* image_surface;
    int limit;
    SDL_Init(SDL_INIT_VIDEO);
    image_surface = load_image(PATH_IMAGE);
    SDL_Window* window = SDL_CreateWindow("window", 0, 0, image_surface->w, image_surface->h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int width = image_surface->w;
    int* Case = calloc(sizeof(int),width * image_surface->h);
    for(int i = 5; i<square1[0]-5; i++)
    {
        Case[i] = 1;
	Case[i+width] = 1;
	limit = i+width;
    }
    for(int i = 5;i<=limit;i += width)
    {
	Case[i] = 1;
	Case[i+square1[0]] = 1;
    }
    drawSide(image_surface,Case);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer,image_surface);
    display_image(renderer, texture);
    wait_for_keypressed();
    fillPoly(image_surface,Case);
    texture = SDL_CreateTextureFromSurface(renderer,image_surface);
    display_image(renderer, texture);
    wait_for_keypressed();
    free(Case);
    SDL_FreeSurface(image_surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
