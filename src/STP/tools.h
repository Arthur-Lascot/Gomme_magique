#ifndef TOOLS_H_
#define TOOLS_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


typedef struct Point
{
    int X;
    int Y;
    Point[2] link;
    int nb_link;
} Point;
int comb(int n, int k);
int facto(int n);
void display_image(SDL_Renderer *renderer,SDL_Texture* texture);
SDL_Surface* load_image(const char* path);
void wait_for_keypressed();
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
#endif
