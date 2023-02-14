#include <stdio.h>
#include "../src/STP/selection_to_pixel.h"
#include "../src/STP/tools.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

char *PATH_IMAGE = "ressources/image_test_1.jpeg";
int square1[2] = {200,100};
int square2[2] = {400,300};
int main()
{
    SDL_Surface* image_surface;
    SDL_Init(SDL_INIT_VIDEO);
    image_surface = load_image(PATH_IMAGE);
    SDL_Window* window = SDL_CreateWindow("window", 0, 0, image_surface->w, image_surface->h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int width = image_surface->w;
    int height = image_surface->h;
    int* Case = calloc(sizeof(int),width * height);
    int ligne_offset = 200;
    int column_offset = 200;
    for(int i = column_offset; i<column_offset+square1[0]; i++)
    {
        Case[i+(ligne_offset*width)] = 1; //(colone 200->200+square1[0] , ligne 200)
	    Case[i+((ligne_offset+square1[1])*width)] = 1; //colone 200->200+suqare1[0] , 200+square1[1] ligne      = 2 + 3x = 17
        /*[ [0,0,0,0,0] 
            [0,0,0,0,0]
            [0,0,0,1,0] 
            [0,0,1,0,0]
            [0,0,0,0,0] ]
            */
    }
    int j = 0;
    for(int i = column_offset;j<square1[1];i += width)
    {
	    j++;
	    Case[i+(ligne_offset*width)] = 1; 
	    Case[i+square1[0]+(ligne_offset*width)] = 1; // = (1 + width) + 2 +width = 2x + 3 = 13
    }
    drawSide(image_surface,Case);
    SDL_Texture* texture = 
    SDL_CreateTextureFromSurface(renderer,image_surface);
    display_image(renderer, texture);
    wait_for_keypressed();
    SDL_DestroyTexture(texture);
    fillPoly(image_surface,Case);
    texture = SDL_CreateTextureFromSurface(renderer,image_surface);
    display_image(renderer, texture);
    wait_for_keypressed();
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image_surface);


    image_surface = load_image(PATH_IMAGE);
    width = image_surface->w;
    height = image_surface->h;
    Case = calloc(sizeof(int),width * height);
    for(int i = 200; i<200+square1[0]; i++)
    {
        Case[i+(200*width)] = 1;
	    Case[i+((200+square1[1])*width)] = 1;
    }
    j = 0;
    for(int i = 200;j<square1[1];i += width)
    {
	    j++;
	    Case[i+(200*width)] = 1;
	    Case[i+square1[0]+(width*200)] = 1;
    }
    for(int i = 100; i<100+square2[0]; i++)
    {
        Case[i+(100*width)] = 1;
	    Case[i+((100+square2[1])*width)] = 1;
    }
    j = 0;
    for(int i = 100;j<square2[1];i += width)
    {
	    j++;
	    Case[i+(100*width)] = 1;
	    Case[i+square2[0]+(width*100)] = 1;
    }
    drawSide(image_surface,Case);
    texture = SDL_CreateTextureFromSurface(renderer,image_surface);
    display_image(renderer, texture);
    wait_for_keypressed();
    SDL_DestroyTexture(texture);
    fillPoly(image_surface,Case);
    texture = SDL_CreateTextureFromSurface(renderer,image_surface);
    display_image(renderer, texture);
    wait_for_keypressed();
    SDL_DestroyTexture(texture);

    free(Case);
    SDL_FreeSurface(image_surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
