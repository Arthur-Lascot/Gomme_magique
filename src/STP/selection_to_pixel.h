#ifndef SELECTION_TO_PIXEL_H_
#define SELECTION_TO_PIXEL_H_
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>

void drawSide(SDL_Surface* image_surface,int *Case);
int isSurrouned(int pixel,int* Case,int width, int height);
int checkFormat(SDL_Surface* image_surface, int* Case);
int isInPoly(int pixel,int* Case,int width);
int fillLine(SDL_Surface* image_surface, int* Case, int index);
int* fillPoly(SDL_Surface* image_surface, int* Case);

#endif
