#include <stdio.h>
#include "../src/selection_to_pixel.h"
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

char *PATH_IMAGE = "../ressources/image_test_1";
(int L,int l) square1 = (20,10);
(int L,int l)square2 = (40,20);
int main()
{
    SDL_Surface* image_surface;
    init_sdl();
    image_surface = load_image(PATH_IMAGE);
    int* case = calloc(sizeof(int),image_surface->w * image_surface->h);
    for(int i = 5, i<square1.L-5, i++)
    {
        
    }
    drawSide(image_surface,case);
    display_image(image_surface);
    wait_for_keypressed();
    fillPoly(image_surface,case);
    display_image(image_surface);
    wait_for_keypressed();
    free(case);
    SDL_FreeSurface(image_surface);
}