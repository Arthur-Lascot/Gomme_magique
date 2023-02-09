#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

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

SDL_Surface *load_image(const char *path);

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

void fill_hole(SDL_Surface *surface)
{
    Uint32 *pixels = surface->pixels;
    size_t w = surface->w;
    size_t h = surface->h;
    SDL_PixelFormat *format = surface->format;
    SDL_LockSurface(surface);
    char tab[h * w];
    for (size_t i = 0; i < h * w; i++)
    {
        tab[i] = 0;
    }
    for (size_t i = 0; i < w; i++)
    {
        for (size_t j = 0; j < h; j++)
        {
            if (tab[j + i * h] == 0)
            {
                Uint8 r, g, b;
                SDL_GetRGB(pixels[j + i * h], format, &r, &g, &b);
                rec_to_image(pixels, format, i, j, tab, h, w, r, g, b);
            }
        }
    }
    SDL_UnlockSurface(surface);
}

// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface *load_image(const char *path)
{
    SDL_Surface *surface = IMG_Load(path);
    return SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
}

int main(int argc, char **argv)
{
    // Checks the number of arguments.
    if (argc > 3)
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

    fill_hole(surface);
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
