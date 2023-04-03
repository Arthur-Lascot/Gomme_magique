#ifndef SELECTION_TO_PIXEL_H_
#define SELECTION_TO_PIXEL_H_
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_image.h>
#include "tools.h"

void bresenham(Point *a, Point *b, int* map, int w);
int* drawBorder(int* raw_points, size_t nb_points, SDL_Surface* image_surface);
void drawSide(SDL_Surface* image_surface,int *Case);
int isSurrouned(int pixel,int* Case,int width, int height);
int checkFormat(SDL_Surface* image_surface, int* Case);
int isInPoly(int pixel,int* Case,int width);
int fillLine(SDL_Surface* image_surface, int* Case, int index);
int isLineValid(int *Case, int w, int i);
void fillPoly(SDL_Surface *image_surface, int *Case);

#endif
