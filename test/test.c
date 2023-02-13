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
    int limit;
    SDL_Init(SDL_INIT_VIDEO);
    image_surface = load_image(PATH_IMAGE);
    SDL_Window* window = SDL_CreateWindow("window", 0, 0, image_surface->w, image_surface->h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    int width = image_surface->w;
    int height = image_surface->h;
    int* Case = calloc(sizeof(int),width * height);
    for(int i = 100; i<100+square1[0]; i++)
    {
        Case[i+square1[1]*width] = 1;
	    Case[i+(2*square1[1]*width)] = 1;
	    limit = i+width;
    }
    int j = 0;
    for(int i = 100;j<square1[1];i += width)
    {
	    j++;
	    Case[i+(width*square1[1])] = 1;
	    Case[i+limit-100+(width*square1[1])] = 1;
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
    int width = image_surface->w;
    int height = image_surface->h;
    int* Case = calloc(sizeof(int),width * height);
    for(int i = 200; i<200+square1[0]; i++)
    {
        Case[i+(200*width)] = 1;
	    Case[i+((200+square1[1])*width)] = 1;
    }
    int j = 0;
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
    int j = 0;
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
