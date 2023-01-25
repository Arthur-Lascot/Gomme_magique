#include <stdio.h>
#include "../src/selection_to_pixel.h"
#include "../src/base_function_on_pict.h"
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

char *PATH_IMAGE = "../ressources/image_test_1.jpeg";
int square1[2] = {20,10};
int square2[2] = {40,20};
int main()
{
    SDL_Surface* image_surface;
    int limit;
    init_sdl();
    image_surface = load_image(PATH_IMAGE);
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
    display_image(image_surface);
    wait_for_keypressed();
    fillPoly(image_surface,Case);
    display_image(image_surface);
    wait_for_keypressed();
    free(Case);
    SDL_FreeSurface(image_surface);
}
