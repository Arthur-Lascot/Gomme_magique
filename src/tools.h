#ifndef TOOLS_H_
#define TOOLS_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void display_image(SDL_Renderer renderer,SDL_Texture* texture);

SDL_Surface* load_image(const char* path);

#endif
