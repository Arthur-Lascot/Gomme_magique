#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tools.h"



int comb(int n, int k)
{
    if(n<k)
        errx(EXIT_FAILURE,"comb: n muts be greater than k");
    int res = facto(n);
    res = res / (facto(k) * facto(n-k));
    return res;
}

int facto(int n)
{
    if(n<0)
        errx(EXIT_FAILURE,"facto: n must be positive");
    int res = 1;
    for(int i = 2; i<=n; i++)
    {
        res = res * i;
    }
    return res;
}

void display_image(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface= IMG_Load(path);
    return surface;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}
//Uint32* pixels = surface->pixels;
//SDL_GetRGB(pixels[j+i*h], format, &r, &g, &b);
/*if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Surface* image_surface= load_image(argv[1]);

    SDL_Window* window = 
	    SDL_CreateWindow("My picture", 0, 0, image_surface->w, image_surface->h,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError())

    SDL_Renderer* renderer = 
	    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    SDL_Texture* textureg= SDL_CreateTextureFromSurface(renderer,surface);

    SDL_FreeSurface( surface);

    // - Dispatch the events.
	event_loop(renderer, texture, textureg);
   
    // - Destroy the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
            */
