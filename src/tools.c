#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>



void display_image(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface= IMG_Load(path);
    return SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888,0);
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