#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "../STP/selection_to_pixel.h"
#include "../STP/tools.h"

#define DEBUG 1

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer *renderer, SDL_Texture *texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// before_img: Texture that contains the before_img image.
// after_img: Texture that contains the after_img image.
void event_loop(SDL_Renderer *renderer, SDL_Texture *before_img, SDL_Texture *after_img)
{
    SDL_Event event;
    SDL_Texture *t = before_img;
    draw(renderer, t);
    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        // If the "quit" button is pushed, ends the event loop.
        case SDL_QUIT:
            return;

        // If the window is resized, updates and redraws the diagonals.
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                draw(renderer, t);
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_a)
            {
                if (t == before_img)
                    t = after_img;
                else
                    t = before_img;
                draw(renderer, t);

                // if (t == before_img)
                // {
                //     t = after_img;
                //     draw(renderer, t);
                // }
                // else
                // {
                //     t = before_img;
                //     draw(renderer, t);
                //     //	printf("changer\n");
                // }
            }
            if (event.key.keysym.sym == SDLK_LEFT)
            {
            }
            break;
        }
    }
}

Uint32 pixel_to_black(Uint32 pixel_color, SDL_PixelFormat *format)
// convert a pixel on a black pixel
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel_color, format, &r, &g, &b);
    int average = 0.0 * r + 0.0 * g + 0.0 * b;
    return SDL_MapRGB(format, average, average, average);
}

/// @brief deep copy of pixel
Uint32 copy_pixel(Uint32 pixel, SDL_PixelFormat *format)
{

    Uint8 r, g, b;
    SDL_GetRGB(pixel, format, &r, &g, &b);
    return SDL_MapRGB(format, r, g, b);
}


void place_pixel(SDL_Surface *surface, int tab[], int x, int y)
{
    Uint32 *pixels = surface->pixels;
    size_t w = surface->w;
    size_t h = surface->h;
    /*if (tab[x + (y-1) * w] == 0)
        put_pixel(surface, x, y, pixels[x + (y-1) * w]);
    else if (tab[x-1 + (y-1) * w] == 0)
        put_pixel(surface, x, y, pixels[x-1 + (y-1) * w]);
    else if (tab[x+1 + (y-1) * w] == 0)
        put_pixel(surface, x, y, pixels[x+1 + (y-1) * w]);
    else if (tab[x-1 + y * w] == 0)
        put_pixel(surface, x, y, pixels[x-1 + y * w]);

    else if (tab[x+1 + y * w] == 0)
        put_pixel(surface, x, y, pixels[x+1 + y * w]);
    else if (tab[x-1 + (y+1) * w] == 0)
        put_pixel(surface, x, y, pixels[x-1 + (y+1) * w]);
    else if (tab[x + (y+1) * w] == 0)
        put_pixel(surface, x, y, pixels[x + (y+1) * w]);
    else if (tab[x+1 + (y+1) * w] == 0)
        put_pixel(surface, x, y, pixels[x+1 + (y+1) * w]);*/

    int pixel = x + y * w;

    int pos[8];
    pos[0] = pixel-w-1;
    pos[1] = pixel-w;
    pos[2] = pixel-w+1;
    pos[3] = pixel+1;
    pos[4] = pixel+w+1;
    pos[5] = pixel+w;
    pos[6] = pixel+w-1;
    pos[7] = pixel-1;

    Uint32 col[] = {0,0,0,0,0,0,0,0};
    int nb[] = {0,0,0,0,0,0,0,0};
    for (size_t i = 0; i < 8; i++)
    {
        if (tab[pos[i]] != 0)
            continue;
        for (size_t c = 0; c < 8; c++) {
            if (col[c] == 0) {
                col[c] = pixels[pos[i]];
            }
            else if (col[c] == pixels[pos[i]]) {
                nb[c]++;
                break;
            }
        }
    }
    int nbmax = 0;
    int imax = 0;
    for (size_t i = 0; i < 8; i++)
    {
        if (nb[i]>nbmax) {
            nbmax = nb[i];
            imax = i;
        }
    }
    put_pixel(surface, x, y, col[imax]);
    tab[x + y * w] = 0;
}

void fill_hole(SDL_Surface *surface, int tab[])
{
    size_t w = surface->w;
    size_t h = surface->h;
    SDL_LockSurface(surface);

    int sens = 0;
    int xd = 1;
    int yd = 0;
    size_t X = 0;
    size_t Y = 0;

    //printf("tab[100 + 100 * w] : %d\n", tab[100 + 100 * w]);

    for (size_t i = 0; i < w*h; i++) {
        if (tab[i] == 1) {
            X = i%w;
            Y = i/w;
            break;
        }
    }

    while (tab[X + Y * w] != 0)
    {
        place_pixel(surface, tab, X, Y);
        //printf("hello %ld %ld\n", X, Y);

        if (tab[(X + xd) + (Y + yd) * w] != 1)
        {
            switch (sens)
            {
            case 0: // droite
                xd = 0;
                yd = 1;
                break;
            case 1: // bas
                xd = -1;
                yd = 0;
                break;
            case 2: // gauche
                xd = 0;
                yd = -1;
                break;
            case 3: // haut
                xd = 1;
                yd = 0;
                break;

            default:
                break;
            }
            sens++;
            sens %= 4;
        }
        X += xd;
        Y += yd;
    }

    SDL_UnlockSurface(surface);
}

/*// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface *load_image(const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    return SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
}*/

int main(int argc, char **argv)
{
    // Checks the number of arguments.
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");
    
    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a window.
    SDL_Window *window =
        SDL_CreateWindow("My picture", 0, 0, 640, 640,
                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());

    // - Create a renderer.
    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    

    // - Create a surface from the colored image.
    SDL_Surface *surface = load_image(argv[1]);
    // - Resize the window according to the size of the image.
    // SDL_SetWindowSize(renderer, surface->w,surface->h);


    // - Create a before_texture from the before surface.
    SDL_Texture *before_texture = SDL_CreateTextureFromSurface(renderer, surface);
    int width = surface->w;
    int height = surface->h;

    int *Case = calloc(sizeof(int), width * height);
    int square1[2] = {200,100};
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

    fillPoly(surface, Case);

    // drawSide(surface, Case);

    fill_hole(surface, Case);

    free(Case);

    // - Create a new texture from the after surface.
    SDL_Texture *after_texture = SDL_CreateTextureFromSurface(renderer, surface);

    // - Free the surface.
    SDL_FreeSurface(surface);

    // - Dispatch the events.
    event_loop(renderer, before_texture, after_texture);

    // - Destroy the objects.
    SDL_DestroyTexture(before_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
