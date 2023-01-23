#ifndef SELECTION_TO_PIXEL_H_
#define SELECTION_TO_PIXEL_H_
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

SDL_Surface* drawSide(SDL_Surface* image_surface,int* case);
bool isSurrouned(int pixel,int* case,int width, int height);
bool checkFormat(SDL_Surface* image_surface, int* case);
bool isInPoly(int pixel,int* case,int width);
void fillLine(SDL_Surface image_surface, int* case, int index);
SDL_surface* fillPoly(SDL_Surface* image_surface, int* case);

#endif