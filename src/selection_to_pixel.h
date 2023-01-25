#ifndef SELECTION_TO_PIXEL_H_
#define SELECTION_TO_PIXEL_H_
#include <stdlib.h>
#include <SDL/SDL.h>
#include <stdbool.h>
#include <SDL/SDL_image.h>

SDL_Surface* drawSide(SDL_Surface* image_surface,int *Case);
bool isSurrouned(int pixel,int* Case,int width, int height);
bool checkFormat(SDL_Surface* image_surface, int* Case);
bool isInPoly(int pixel,int* Case,int width);
void fillLine(SDL_Surface* image_surface, int* Case, int index);
SDL_Surface* fillPoly(SDL_Surface* image_surface, int* Case);

#endif
