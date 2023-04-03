#include <stdio.h>
#include "tools.h"
#include "selection_to_pixel.h"
#include <err.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

void bresenham(Point *a, Point *b, int* map, int w)
{
    int dx = b->X - a->X;
    int dy = b->Y - a->Y;
    int x1 = a->X;
    int y1 = a->Y;
    int x2 = b->X;
    int y2 = b->Y;
    printf("x1 = %d\ny1 = %d\nx2 = %d\ny2 = %d\n",x1,y1,x2,y2);

    if(dx!=0)
    {
        if(dx>0)
        {
            if(dy!=0)
            {
                if(dy>0)
                {
                    if(dx>=dy)
                    {
                        int e = dx;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((x1)!=x2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            x1++;
                            e = e - dy;
                            if(e < 0)
                            {
                                y1++;
                                e = e + dx;
                            }
                        }
                    }
                    else
                    {
                        int e = dy;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((y1)!=y2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            y1++;
                            e = e - dx;
                            if(e < 0)
                            {
                                x1++;
                                e = e + dy;
                            }
                        }
                    }
                }
                else
                {
                    if(dx >= -dy)
                    {
                        int e = dx;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((x1)!=x2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            x1++;
                            e = e + dy;
                            if(e < 0)
                            {
                                y1--;
                                e = e + dx;
                            }
                        }
                    }
                    else
                    {
                        int e = dy;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((y1)!=y2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            y1--;
                            e = e + dx;
                            if(e > 0)
                            {
                                x1++;
                                e = e + dy;
                            }
                        }
                    }
                }
            }
            else
            {
                while(x1!=x2)
                {
                    *(map+x1+(y1*w)) = 1;
                    x1++;
                }
            }
        }
        else
        {
            if(dy != 0)
            {
                if(dy > 0)
                {
                    if(-dx >= dy)
                    {
                        int e = dx;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((x1)!=x2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            x1--;
                            e = e + dy;
                            if(e >= 0)
                            {
                                y1++;
                                e = e + dx;
                            }
                        }
                    }
                    else
                    {
                        int e = dy;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((y1)!=y2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            y1++;
                            e = e + dx;
                            if(e <= 0)
                            {
                                x1--;
                                e = e + dy;
                            }
                        }
                    }
                }
                else
                {
                    if(dx <= dy)
                    {
                        int e = dx;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((x1)!=x2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            x1--;
                            e = e - dy;
                            if(e >= 0)
                            {
                                y1--;
                                e = e + dx;
                            }
                        }
                    }
                    else
                    {
                        int e = dy;
                        dx = dx * 2;
                        dy = dy * 2;
                        while((y1)!=y2)
                        {
                            *(map+x1+(y1*w)) = 1;
                            y1--;
                            e = e - dx;
                            if(e >= 0)
                            {
                                x1--;
                                e = e + dy;
                            }
                        }
                    }
                }
            }
            else
            {
                while(x1 != x2)
                {
                    *(map+x1+(y1*w)) = 1;
                    x1--;
                }
            }
        }
    }
    else
    {
        if(dy != 0)
        {
            if(dy > 0)
            {
                while(y1 != y2)
                {
                    *(map+x1+(y1*w)) = 1;
                    y1++;
                }
            }
            else
            {
                while(y1 != y2)
                {
                    *(map+x1+(y1*w)) = 1;
                    y1--;
                }
            }
        }
    }
    *(map+x1+(y1*w)) = 1;
}

int* drawBorder(int* raw_points, size_t nb_points, SDL_Surface* image_surface)
{
    int w = image_surface->w;
    int h = image_surface->h;
    int* map = calloc(w*h,sizeof(int));
    //Formating an array of Point
    ///////////////////////////////////////////////////////////
    /*Point **points = malloc(sizeof(int) * nb_points);
    for(size_t i = 0; i<nb_points*2; i+=2)
    {
        points[i]->X = *(raw_points + i);
        points[i]->Y = *(raw_points + i + 1);
    }*/
    //////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////
    Point **points = malloc(sizeof(Point *) * nb_points);
    for(size_t i = 0; i<nb_points; i++)
    {
        points[i] = malloc(sizeof(Point));
        points[i]->X = *(raw_points + i*2);
        points[i]->Y = *(raw_points + i*2 + 1);
    }
    //////////////////////////////////////////////////////////
    for(size_t i = 0; i<nb_points-1; i++)
    {
        bresenham(points[i], points[i+1], map, w);
    }
    bresenham(points[0], points[nb_points-1], map, w);
    /*for (size_t i = 0; i < nb_points; i++)
        free(points(i));//besoin de crochet
    free(points);*/
    return map;
}

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
            pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
            put_pixel(image_surface, column, line, pixel);
        }
        if(*(Case+i) == 2)
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
    int enchainement = 0;
    for(int i = 0; i<width; i++)
    {
        if(Case[pixel+i] == 1)
        {
            count+=1;
            enchainement += 1;
        }
        else
            enchainement = 0;
        if (enchainement >= 2)
        {
            count -= 1;
        }
        
    }
    if(count%2 != 0 && count<5)
    {
        return 0;
    }

    return 1;
}

int fillLine(SDL_Surface* image_surface, int* Case, int index)
{
    int width = image_surface->w;
    int line;
    int column;
    //Uint32 pixel;
    column = index%width;
    int value = isInPoly(index, Case, width-column);
    if(value==0)
    {
        //Case[index] = 1;
        line = index / width;
        if(Case[column + line*width]!=1) {
            Case[column + line*width] = 2;
            //printf("x = %d\ty = %d\n", column, line);
        }

        //pixel = get_pixel(image_surface, column, line);
        //pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
        //put_pixel(image_surface, column, line, pixel);
    }
    return value;
}
int isLineValid(int* Case, int w, int i)
{
    int max;
    int enchainement = 0;
    int count = 0;
   
    max = i + w;
    for (int j = i; j < max; j++) {
        if(Case[i+j] == 1)
        {
            count+=1;
            enchainement += 1;
        }
        else {
            enchainement = 0;
        }
        if (enchainement > 1)
        {
            count -= 1;
        }
    }
    return count > 1;
}
void fillPoly(SDL_Surface* image_surface, int* Case)
{
    int width = image_surface->w;
    int height = image_surface->h;
    /*if(checkFormat(image_surface, Case)!=0)
    {
	SDL_FreeSurface(image_surface);
        errx(1, "checkFormat : Pixel selection format is wrong");
    }*/
    int max;
    int count = 0;
    int test = 1;
    for(int i = 0; i<width*height; i++)
    {   
        if(i%width == 0)
        {
            max = i + width;
            for (int j = 0; j+i < max; j++) {
                if(Case[i+j]!=1)
                    test = 1;
                if(Case[i+j]==1 && test == 1) {
                    count += 1;
                    test = 0;
                }
            }
            //printf("count = %d\n",count);
            if(count == 1) {
                //puts("je suis utile");
                i += width - 1;
                count = 0;
                test = 1;
                continue;
            }
            count = 0;
            test = 1;
        }
            
        fillLine(image_surface, Case, i);

    }
}
