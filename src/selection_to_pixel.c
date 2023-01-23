#include <stdio.h>
#include "base_function_on_pict.h"
#include "selection_to_pixel.h"
#include <err.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

SDL_Surface* drawSide(SDL_Surface* image_surface,int* case)
{
    int width = image_surface->w;
    int height = image_surface->h;

    Uint32 pixel;
    Uint8 r, g, b;

    int line;
    int column;

    for(int i = 0, i<width*height, i++)
    {
        if(*(case+i) == 1)
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

bool isSurrouned(int pixel,int* case,int width, int height)
{
    int pos[8];
    pos[0] = i-width-1;
    pos[1] = i-width;
    pos[2] = i-width+1;
    pos[3] = i+1;
    pos[4] = i+width+1;
    pos[5] = i+width;
    pos[6] = i+width-1;
    pos[7] = i-1;

    for(int i = 0, i<8, i++)
    {
        if(pos[i]>=0 && pos[i]<=width*height)
        {
            if(case[pos[i]]==1)
            {
                return true;
            }
        }
    }
    return false;    
}
bool checkFormat(SDL_Surface* image_surface, int* case)
{
    int width = image_surface->w;
    int height = image_surface->h;

    for(int i = 0, i<width*height, i++)
    {
        if(!isSurrouned(i,case,width,height))
        {
            return false;
        }
    }
    return true;
}

bool isInPoly(int pixel,int* case,int width)
{
    int count = 0;
    
    for(int i = 1, i<width, i++)
    {
        if(case[pixel+i] == 1)
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

void fillLine(SDL_Surface image_surface, int* case, int index)
{
    int width = image_surface->w;
    int line;
    int column;

    for(int i = 0, i<width, i++)
    {
        if(isInPoly(i, case, width-i))
        {
            line = i/width;
            column = i%width;
            pixel = get_pixel(image_surface,column,line);
            pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
            put_pixel(image_surface, column, line, pixel);
        }
    }
}

SDL_surface* fillPoly(SDL_Surface* image_surface, int* case)
{
    int width = image_surface->w;

    if(!checkFormat)
    {
        errx(1, "checkFormat : Pixel selection format is wrong");
    }

    for(int i = 0, i<width*height, i+=width)
    {
        fillLine(image_surface,case,i);   
    }
}