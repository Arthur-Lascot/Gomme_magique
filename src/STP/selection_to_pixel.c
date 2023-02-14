#include <stdio.h>
#include "tools.h"
#include "selection_to_pixel.h"
#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void drawSide(SDL_Surface* image_surface,int* Case)
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
            pixel = SDL_MapRGB(image_surface->format, 0, 0, 255);
            put_pixel(image_surface, column, line, pixel);
        }
    }
}

int isSurrouned(int pixel,int* Case,int width, int height)
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
                return 0;
            }
        }
    }
    return 1;    
}
int checkFormat(SDL_Surface* image_surface, int* Case)
{
    int width = image_surface->w;
    int height = image_surface->h;

    for(int i = 0; i<width*height; i++)
    {
        if(isSurrouned(i,Case,width,height)!=0)
        {
            return 1;
        }
    }
    return 0;
}

int isInPoly(int pixel,int* Case,int width)
{
    int count = 0;
    int isprev = 0;
    for(int i = 1; i<width; i++)
    {
        if(Case[pixel+i] == 1)
        {
            count+=1;
	    isprev += 1;
        }
	else
	{
		isprev = 0;
	}
	if(isprev>=4)
	{
		break;
	}
    }

    if(count%2 != 0)
    {
        return 0;
    }

    return 1;
}

void fillLine(SDL_Surface* image_surface, int* Case, int index)
{
    int width = image_surface->w;
    int line;
    int column;
    Uint32 pixel;
    column = index%width;
    if(isInPoly(index, Case, width-column)==0)
    {
        //Case[index] = 1;
        line = index / width;
        Case[column + line*width] = 1;
        //pixel = get_pixel(image_surface, column, line);
        pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, column, line, pixel);
    }
}

int* fillPoly(SDL_Surface* image_surface, int* Case)
{
    int width = image_surface->w;
    int height = image_surface->h;

    /*if(checkFormat(image_surface, Case)!=0)
    {
	SDL_FreeSurface(image_surface);
        errx(1, "checkFormat : Pixel selection format is wrong");
    }*/

    for(int i = 0; i<width*height; i+=1)
    {
        fillLine(image_surface,Case,i);   
    }
    return Case;
}
