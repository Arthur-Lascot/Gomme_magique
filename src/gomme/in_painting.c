#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "in_painting.h"
#include "../STP/tools.h"

/// @brief Set true in ret[0] if the entier area psi is in the bound of image AND set x in ret[1] and y in ret[2] (the upper left corner, not the center)
/// @param pixel 
/// @param w Width
/// @param h Height
/// @param ret tab of return : {is valid ?, x, y}
void is_valid(int pixel, int w, int h, int *ret)
{
    int x = pixel % w;
    int y = pixel / w;
    int offset = (PSY_W - 1) / 2;

    x -= offset;
    y -= offset;

    ret[0] = x<0 || y<0 || x+PSY_W>w || y+PSY_W>h;
    ret[1] = x;
    ret[2] = y;
}

/// @brief Compute confidence for pixel
/// @param pixel 
/// @param C Tab of confidence
/// @param w Width
/// @param h Height
/// @return Confidence for pixel
double conf(int pixel, double* C, int w, int h)
{
    int arg[3];
    is_valid(pixel,w,h,arg);

    if (arg[0])
        return 0.0;

    int x = arg[1];
    int y = arg[2];

    double confidence = 0.0;

    int max = (y + PSY_W) * w + (x + PSY_W);
    for (int i = y*w+x; i < max; i++)
    {
        confidence += C[i];

        if (i%w == x+PSY_W)
            i += w - PSY_W;
    }
    confidence /= PSY_W * PSY_W;

    return confidence;
}

/// @warning TODO
/// @brief Compute data term, D(p) in math formula
/// @return data term
double data_term(SDL_Surface *surface, int p, int *map)
{
    //sobel->gradient->sous-vecteur orthogonal . np (valeur absolue)  /255
    const size_t alpha = 255;
    int grad[2];
    get_sobel(surface,p,grad);
    //orthogonal_de_grad()
    np = get_np();
    return 1.0;//FAKE
}

/// @warning ! double type not sure !
/// @brief Compute the distance between  psi of p  and  psi of q
/// @param surface The image sdl surface's
/// @param p Point with the best P(p)
/// @param q Point to test with p to check distance between their psi
/// @param map Tab of status (0:source, 1:border, 2:target)
/// @return The disance
double dist_psi(SDL_Surface *surface, int p, int q, int* map)
{
    int w = surface->w;
    int h = surface->h;

    int arg[3];
    is_valid(q,w,h,arg);

    if (arg[0])
        return __DBL_MAX__;

    int xq = arg[1];
    int yq = arg[2];
    
    int max = (yq + PSY_W) * w + (xq + PSY_W);
    for (int i = yq*w+xq; i < max; i++)
    {

        if (map[i] != 0) // if not in source img
            return __DBL_MAX__;


        if (i%w == xq+PSY_W) // jump line
            i += w - PSY_W;
    }
    double ssd = 0;
    Uint32 pixelInP;
    Uint32 pixelInQ;
    Uint8 r, g, b;
    double valueP;
    double valueQ;
    int linep;
    int columnp;
    int lineq;
    int columnq;
    int offset = (PSY_W - 1) / 2;
    int arg[3];
    is_valid(p,w,h,arg);
    int indexForq = yq*w+xq;
    int diff;
    int xp = arg[1];
    int yp = arg[2];
    int max = (yp + PSY_W) * w + (xp + PSY_W);
    for(int i = yp*w+xp; i < max; i++)
    {
        if(map[i]==0) // if we are in already filled part of patch
        {
            linep = i/w;
            columnp = i%w;
            lineq = indexForq/w;
            columnq = indexForq%w;
            pixelInP = get_pixel(surface,columnp,linep); //Gettting pixel in patch P
            pixelInQ = get_pixel(surface,columnq,lineq); //Getting corresponding pixel in patch Q
	        SDL_GetRGB(pixelInP, surface->format, &r, &g, &b);
            valueP = r + g + b; //Value of pixel P
            SDL_GetRGB(pixelInQ, surface->format, &r, &g, &b);
            valueQ = r + g + b; //Value of pixel Q
            diff = valueP - valueQ;
            ssd += diff * diff; //square of sum difference
        }
        if (i%w == xp+PSY_W) // jump line
        {
            i += w - PSY_W;
            indexForq += w -PSY_W;
        }
        indexForq++;
    }
    return ssd;
}

/// @brief Set the new borders in map at one larger pixel side around
/// @param map Tab of status (0:source, 1:border, 2:target)
/// @param x Upper left x coordinate from p in copy() method
/// @param y Upper left y coordinate from p in copy() method
/// @param w Width
/// @param h Height
void update_border(int* map, int x, int y, int w, int h)
{
    x--;
    y--;
 
    int max = PSY_W + 2;

    if (y<0)
        x += max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || x<0 || x+PSY_W>w))
                map[y * w + x] = 1;
            x++;
        }
    
    if (x+PSY_W>w)
        y += max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || y<0 || y+PSY_W>h))
                map[y * w + x] = 1;
            y++;
        }
    
    if (y+PSY_W>h)
        x -= max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || x<0 || x+PSY_W>w))
                map[y * w + x] = 1;
            x--;
        }
    
    if (x<0)
        return; // y -= max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || y<0 || y+PSY_W>h))
                map[y * w + x] = 1;
            y--;
        }
}

/// @brief Copy pixels of q psi in p psi and update map and C tabs
/// @param surface The image sdl surface's
/// @param C Tab of confidence
/// @param map Tab of status (0:source, 1:border, 2:target)
/// @param p Point with the best P(p)
/// @param q Point with the minimal distance of psi to p psi
/// @param conf_p Confidence of p psi filling
void copy(SDL_Surface *surface, double *C, int *map, int p, int q, double conf_p)
{
    int offset = (PSY_W - 1) / 2;
    int w = surface->w;
    int h = surface->h;
    int xp = p % w;
    int yp = p / w;
    xp -= offset;
    yp -= offset;

    int xq = q % w;
    int yq = q / w;
    xq -= offset;
    yq -= offset;

    int max = (yp + PSY_W) * w + (xp + PSY_W);
    int ip = yp*w+xp;
    int iq = yq*w+xq;

    Uint32 pixel;
    int pline;
    int pcolumn;
    int qline;
    int qcolumn;
    while (ip < max /*&& iq < maxq*/)
    {
        if (map[ip] != 0) {
            C[ip] = conf_p;
            map[ip] = 0;

            pline = ip / w;
            pcolumn = ip % w;
            qline = iq / w;
            qcolumn = iq % w;
            pixel = get_pixel(surface, qcolumn, qline);
            put_pixel(surface, pcolumn, pline, pixel);
        }

        if (ip%w == xp+PSY_W) // jump line
            ip += w - PSY_W;

        ip++;
        iq++;
    }
    update_border(map, xp, yp, w, h);
}

/// @brief One step of inPainting algorithm
/// @param surface The image sdl surface's
/// @param map Tab of status (0:source, 1:border, 2:target)
void inPainting(SDL_Surface *surface, int* map)
{
    const int w = surface->w;
    const int h = surface->h;
    const int len = w * h;
    double C[len];
    //initialization of the array of confidence
    for(int i = 0; i<len; i++)
    {
        if(map[i]==0)
            C[i] = 1; // C(pixel) = 1 for pixel in (I - Omega)
        else
            C[i] = 0; // C(pixel) = 0 for pixel in (Omega)
    }

    double tmp;
    double tmp_conf;
    double max = 0.0;
    double max_conf = 0.0;
    double max_p;
    // finding the priority values
    for(int p = 0; p<len; p++)
    {
        if(map[p]==1)// points of the edge
        {
            tmp_conf = conf(p, C, w, h);
            tmp = tmp_conf; //* data_term() TODO
            if (tmp>max) {
                max = tmp;
                max_p = p;
                max_conf = tmp_conf;
            }
        }
    }

    //double tmp;
    double min = __DBL_MAX__;
    double min_d;
    for(int q = 0; q<len; q++)
    {
        if(map[q]==0) {
            tmp = dist_psi(surface, max_p, q, map);
            if (tmp<min) {
                min = tmp;
                min_d = q;
            }
        }
    }

    copy(surface, C, map, max_p, min_d, max_conf);
}
