#include <stdio.h>
#include "tools.h"
#include "selection_to_pixel.h"
#include <err.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Surface* drawSide(SDL_Surface* image_surface,int* Case)
{
    int width = image_surface->w;
    int height = image_surface->h;

    Uint32 pixel;

    int line;
    int column;

    for(int i = 0; i<width*height; i++)
    {
        if(*(Case+i) == 1)
        {
            line = i/width;
            column = i%width;
            pixel = get_pixel(image_surface,column,line);
            pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
            put_pixel(image_surface, column, line, pixel);
        }
    }
    return image_surface;
}

bool isSurrouned(int pixel,int* Case,int width, int height)
{
    int pos[8];
    pos[0] = pixel-width-1;
    pos[1] = pixel-width;
    pos[2] = pixel-width+1;
    pos[3] = pixel+1;
    pos[4] = pixel+width+1;
    pos[5] = pixel+width;
    pos[6] = pixel+width-1;
    pos[7] = pixel-1;

    for(int i = 0; i<8; i++)
    {
        if(pos[i]>=0 && pos[i]<=width*height)
        {
            if(Case[pos[i]]==1)
            {
                return false;
            }
        }
    }
    return true;    
}
bool checkFormat(SDL_Surface* image_surface, int* Case)
{
    int width = image_surface->w;
    int height = image_surface->h;

    for(int i = 0; i<width*height; i++)
    {
        if(isSurrouned(i,Case,width,height))
        {
            return true;
        }
    }
    return false;
}

bool isInPoly(int pixel,int* Case,int width)
{
    int count = 0;
    
    for(int i = 1; i<width; i++)
    {
        if(Case[pixel+i] == 1)
        {
            count+=1;
        }
    }

    if(count%2 != 0)
    {
        return true;
    }

    return false;
}

void fillLine(SDL_Surface* image_surface, int* Case, int index)
{
    int width = image_surface->w;
    int line;
    int column;
    Uint32* pixels = image_surface->pixels;
    column = index%width;
    if(isInPoly(index, Case, width-column))
    {
        line = index/width;
        pixels[index] = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, column, line, pixel);
    }
}

SDL_Surface* fillPoly(SDL_Surface* image_surface, int* Case)
{
    int width = image_surface->w;
    int height = image_surface->h;

    if(checkFormat)
    {
        errx(1, "checkFormat : Pixel selection format is wrong");
    }

    for(int i = 0; i<width*height; i+=width)
    {
        fillLine(image_surface,Case,i);   
    }
}
