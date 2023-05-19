#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "in_painting.h"
#include "../STP/tools.h"
#include <math.h>
#include "pthread.h"
#include <time.h> //used for testing

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

    ret[0] = !(x<0 || y<0 || x+PSY_W>w || y+PSY_W>h);
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

    if (!arg[0])
        return 2.0;

    int x = arg[1];
    int y = arg[2];

    double confidence = 0.0;

    int max = (y + PSY_W-1) * w + (x + PSY_W-1);
    for (int i = y*w+x; i <= max; i++)
    {
        confidence += C[i];

        if (i%w == x+PSY_W-1)
            i += w - PSY_W;
    }
    confidence /= PSY_W * PSY_W;

    return confidence;
}

void get_sobel(SDL_Surface *surface, int point, int *grad)
{
    int matX[] = {1, 0, -1,
                  2, 0, -2,
                  1, 0, -1};

    int matY[] = {1, 2, 1,
                  0, 0, 0,
                  -1, -2, -1};

    int w = surface->w;
    int point_mat = 0;
    point -= w + 1;
    Uint8 r, g, b;
    grad[0] = 0;
    grad[1] = 0;
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            Uint32 pixel = get_pixel(surface, point % w, point / w);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            int tmprgb = (r + g + b) / 3;
            grad[0] += tmprgb * matX[point_mat];
            grad[1] += tmprgb * matY[point_mat];
            point++;
            point_mat++;
        }
        point += w - 4;
    }
    
}

void get_np(int x1, int y1, int x2, int y2, double* np)
{
    int tangente[] = {x2-x1,y1-y2};
    double coeff = sqrt(tangente[1] * tangente[1] + (-tangente[0])*(-tangente[0]));
    np[0] = ((double)tangente[1])/coeff;
    np[1] = ((double)(-tangente[0]))/coeff;
}

/// @warning TODO
/// @brief Compute data term, D(p) in math formula
/// @return data term
double data_term(SDL_Surface *surface, int p, int *map)
{
    //sobel->gradient->sous-vecteur orthogonal . np (valeur absolue)  /255
    int w = surface->w;
    int h = surface->h;
    const size_t alpha = 255;
    int grad[2];
    int gradOGN[2];//orthogonal_de_grad()
    double np[2]; //normal vector
    int arg[3];
    is_valid(p,w,h,arg);
    if (!arg[0])
        return -2.0;
    int x = arg[1] + 1;
    int y = arg[2] + 1;
    int max = (y + PSY_W - 2) * w + (x + PSY_W - 2);

    double normMax = 0;
    double norme_tmp;
    for (int i = y*w+x; i < max; i++)
    {

        if (map[i] == 0 && map[i-w-1] == 0 && map[i-w] == 0 && map[i-w+1] == 0 
            && map[i+1] == 0 && map[i+w+1] == 0 && map[i+w] == 0 
            && map[i+w-1] == 0 && map[i-1] == 0) 
        {
            get_sobel(surface,p,grad);
            norme_tmp = sqrt(grad[0] * grad[0] + grad[1] * grad[1]);
            if(norme_tmp > normMax)
            {
                normMax = norme_tmp;
                gradOGN[0] = grad[1];
                gradOGN[1] = -grad[0];
            }
        }


        if (i%w == x+(PSY_W-2)) // jump line
            i += w - (PSY_W-2);
    }
    /////////////////////////////////////
    int line = p/w;
    int column = p%w;
    int x1;
    int y1;
    int x2;
    int y2;
    if(map[p-w-1] == 1)
    {
        x1 = column - 1;
        y1 = line - 1;
        if(map[p-1] == 1)
        {
            x1 = column - 1;
            y1 = line; 
        }
        if(map[p-w] == 1)
        {
            x1 = column;
            y1 = line - 1; 
        }
    }         //Tryiny to find the 2 nearest point of P

    if(map[p+w+1] == 1)
    {
        x2 = column + 1;
        y2 = line + 1;
        if(map[p+w] == 1)
        {
            x2 = column;
            y2 = line + 1; 
        }
        if(map[p+1] == 1)
        {
            x2 = column + 1;
            y2 = line; 
        }
    }

    get_np(x1,y1,x2,y2,np);

    ///////////////////////////////////////////
    return (gradOGN[0]*np[0]+gradOGN[1]*np[1])/alpha;
}

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

    if (!arg[0])
        return __DBL_MAX__;

    int xq = arg[1];
    int yq = arg[2];
    
    int max = (yq + PSY_W-1) * w + (xq + PSY_W-1);
    for (int i = yq*w+xq; i <= max; i++)
    {

        if (map[i] != 0) // if not in source img
            return __DBL_MAX__;


        if (i%w == xq+PSY_W-1) // jump line
            i += w - PSY_W;
    }
    double ssd = 0;
    Uint32 pixelInP;
    Uint32 pixelInQ;
    Uint8 r, g, b;
    int linep;
    int columnp;
    int lineq;
    int columnq;
    is_valid(p,w,h,arg);
    int indexForq = yq*w+xq;
    double diff;
    int xp = arg[1];
    int yp = arg[2];
    max = (yp + PSY_W-1) * w + (xp + PSY_W-1);

    //Uint8 valeurr, valeurg, valeurb;
    for(int i = yp*w+xp; i <= max; )
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
            Uint8 averageP =  0.3*r + 0.59*g + 0.11*b;
            //valeurr = r;
            //valeurg = g;
            //valeurb = b;
            SDL_GetRGB(pixelInQ, surface->format, &r, &g, &b);
            Uint8 averageQ =  0.3*r + 0.59*g + 0.11*b;
            //valeurr -= r;
            //valeurg -= g;
            //valeurb -= b;
            //diff = sqrt((double)(valeurr * valeurr + valeurg * valeurg + valeurb * valeurb));
            diff = averageQ - averageP;
            ssd += diff * diff; //square of sum difference
        }
        if (i%w == xp+PSY_W-1) // jump line
        {
            i += w - (PSY_W-1);
            indexForq += w - (PSY_W - 1);
        }
        else {
            indexForq++;
            i++;
        }
    }
    return ssd;
}

void* dist_psi_threaded(void* arg)
{
    dpsi_arg* argpt = (dpsi_arg *)arg;
    double ssd;
    int xd, xd_min, xd_max, yd, yd_min, yd_max, xq, yq;
    xd = argpt->max_p % argpt->surface->w;
    yd = argpt->max_p / argpt->surface->w;
    int delta = PSY_W*EXT;
    xd_min = xd - delta;
    xd_max = xd + delta;
    yd_min = yd - delta;
    yd_max = yd + delta;

    for(int q = argpt->qstart; q<argpt->qend; q++)
    {
        if(argpt->map[q]==0) {
            xq = q % argpt->surface->w;
            yq = q / argpt->surface->w;
            if (0||(xd_min<xq && xq<xd_max && yd_min<yq && yq<yd_max)) {
                ssd = dist_psi(argpt->surface, argpt->max_p, q, argpt->map);
                if (ssd<argpt->min) {
                    argpt->min = ssd;
                    argpt->min_d = q;
                }
            }
        }
    }
    return NULL;
}

/// @brief Set the new borders in map at one larger pixel side around
/// @param map Tab of status (0:source, 1:border, 2:target)
/// @param x Upper left x coordinate from p in copy() method
/// @param y Upper left y coordinate from p in copy() method
/// @param w Width
/// @param h Height
void update_border(SDL_Surface *surface, int* map, int x, int y, int w, int h)
{
    x--;
    y--;
 
    int max = PSY_W + 2;

    if (y<0)
        x += max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || x<0 || x+PSY_W>w)) {
                map[y * w + x] = 1;
                Uint32 pixel = SDL_MapRGB(surface->format, 255, 0, 0);
                put_pixel(surface, x, y, pixel);
            }
                
            x++;
        }
    
    if (x+PSY_W>w)
        y += max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || y<0 || y+PSY_W>h)){
                map[y * w + x] = 1;
                Uint32 pixel = SDL_MapRGB(surface->format, 255, 0, 0);
                put_pixel(surface, x, y, pixel);
            }
            y++;
        }
    
    if (y+PSY_W>h)
        x -= max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || x<0 || x+PSY_W>w)){
                map[y * w + x] = 1;
                Uint32 pixel = SDL_MapRGB(surface->format, 255, 0, 0);
                put_pixel(surface, x, y, pixel);
            }
            x--;
        }
    
    if (x<0)
        return; // y -= max - 1;
    else
        for (int i = 0; i < max; i++) {
            if (!(map[y * w + x]==0 || y<0 || y+PSY_W>h)){
                map[y * w + x] = 1;
                Uint32 pixel = SDL_MapRGB(surface->format, 255, 0, 0);
                put_pixel(surface, x, y, pixel);
            }
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

    int max = (yp + PSY_W-1) * w + (xp + PSY_W-1);
    int ip = yp*w+xp;
    int iq = yq*w+xq;

    Uint32 pixel;
    int pline;
    int pcolumn;
    int qline;
    int qcolumn;
    while (ip <= max /*&& iq < maxq*/)
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

        if (ip%w == xp+PSY_W-1) { // jump line
            ip += w - (PSY_W-1);
            iq += w - (PSY_W-1);
        }
        else {
            ip++;
            iq++;
        }
    }
    update_border(surface, map, xp, yp, w, h);
}

/// @brief One step of inPainting algorithm
/// @param surface The image sdl surface's
/// @param map Tab of status (0:source, 1:border, 2:target)
void inPainting(SDL_Surface *surface, int* map, int w, int h, double *C, int start)
{
    int len = w * h;
    double tmp;
    double tmp_conf;
    double max = -2.0;
    double max_conf = 0.0;
    int max_p = -1;
    // finding the priority values
    for(int p = start; p<len; p++)
    {
        if(map[p]==1)// points of the edge
        {
            tmp_conf = conf(p, C, w, h);
            //if (tmp_conf == 0.0) continue;
            tmp = tmp_conf;//       *data_term(surface, p, map);
            if (tmp > max)
            {
                max = tmp;
                max_p = p;
                max_conf = tmp_conf;
            }
        }
    }
    if (max_p == -1)
        return;
    //double tmp;
    double min = __DBL_MAX__;
    double min_d = __DBL_MAX__;

    size_t nb_threads = 5;

    if (THREADED == 3) {
        int xd, xd_min, xd_max, yd, yd_min, yd_max;
        xd = max_p % w;
        yd = max_p / w;
        int delta = PSY_W*5;
        xd_min = xd - delta > 0 ? xd - delta : 0;
        xd_max = xd + delta < w ? xd + delta : w;
        yd_min = yd - delta > 0 ? yd - delta : 0;
        yd_max = yd + delta < h ? yd + delta : h;

        int i_min = xd_min + yd_min * w;
        int i_max = xd_max + yd_max * w;
        int i_len = i_max - i_min;

        dpsi_arg argpts[NB_THREADS_MAX];
        pthread_t threads[NB_THREADS_MAX];
        size_t part = i_len / nb_threads;

        size_t i = 0;
        for (; i < nb_threads-1; i++) {
            dpsi_arg argpt = 
                {surface, map, max_p, i_min+i*part, i_min+(i+1)*part, min, min_d};
            argpts[i] = argpt;
            pthread_create(
                &threads[i], NULL, dist_psi_threaded, (void *)&argpts[i]);
        }
        dpsi_arg argpt = {surface, map, max_p, i_min+(i)*part, i_max, min, min_d};
        argpts[i] = argpt;
        pthread_create(&threads[i], NULL, dist_psi_threaded, (void*)&argpts[i]);

        for (size_t i = 0; i < nb_threads; i++) {
            pthread_join(threads[i], NULL);
            if (argpts[i].min < min) {
                min = argpts[i].min;
                min_d = argpts[i].min_d;
            }
        }
        
    }

    if (THREADED == 2) {
        dpsi_arg argpts[NB_THREADS_MAX];
        pthread_t threads[NB_THREADS_MAX];
        size_t part = len / nb_threads;

        size_t i = 0;
        for (; i < nb_threads-1; i++) {
            dpsi_arg argpt = 
                {surface, map, max_p, i*part, (i+1)*part, min, min_d};
            argpts[i] = argpt;
            pthread_create(
                &threads[i], NULL, dist_psi_threaded, (void *)&argpts[i]);
        }
        dpsi_arg argpt = {surface, map, max_p, (i)*part, len, min, min_d};
        argpts[i] = argpt;
        pthread_create(&threads[i], NULL, dist_psi_threaded, (void*)&argpts[i]);

        for (size_t i = 0; i < nb_threads; i++) {
            pthread_join(threads[i], NULL);
            if (argpts[i].min < min) {
                min = argpts[i].min;
                min_d = argpts[i].min_d;
            }
        }
        
    }

    if (THREADED == 1) {
        dpsi_arg argpt1 = {surface, map, max_p, 0, len / 2, min, min_d};
        dpsi_arg argpt2 = {surface, map, max_p, len / 2, len, min, min_d};
        pthread_t thread1, thread2;
        pthread_create(&thread1, NULL, dist_psi_threaded, (void *)&argpt1);
        pthread_create(&thread2, NULL, dist_psi_threaded, (void *)&argpt2);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        min = argpt1.min;
        min_d = argpt1.min_d;
        if (argpt1.min>argpt2.min) {
            min = argpt2.min;
            min_d = argpt2.min_d;
        }
    }
    if (THREADED == 0) {
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
    }
    copy(surface, C, map, max_p, min_d, max_conf);
}


void run_inPainting(SDL_Surface *surface, int* map)
{
    const int w = surface->w;
    const int h = surface->h;
    const int len = w * h;
    double C[len];

    clock_t startclock, endclock;
    time_t starttime, endtime;
    double duraclock = 0.;
    double duratime = 0.;

    //initialization of the array of confidence
    for(int i = 0; i<len; i++)
    {
        if(map[i]==0)
            C[i] = 1; // C(pixel) = 1 for pixel in (I - Omega)
        else
            C[i] = 0; // C(pixel) = 0 for pixel in (Omega)
    }
    int not_finished;
    int start = 0;
    int cptsave = 0;
    do {
        if (DEBUG) printf("Lancement d'un patch %d\n", start);
        startclock = clock();
        starttime = time(NULL);
        inPainting(surface, map, w, h, C, start);
        endclock = clock();
        endtime = time(NULL);
        duraclock += (double)(endclock - startclock) / CLOCKS_PER_SEC;
        duratime += (double)(endtime - starttime);
        if (cptsave%30 == 0) {
            SDL_SaveBMP(surface, "image_temp_inPainting.png");
            if (DEBUG) printf("Temp img saved\n");
        }

        not_finished = 0;
        for (int i = 0; i < len; i++)
        {
            if (map[i] != 0) {
                not_finished = 1;
                start = i;
                break;
            }
        }
        cptsave++;
    } while (not_finished);
    SDL_SaveBMP(surface, "image_temp_inPainting.png");
    if (DEBUG) printf("Temp img saved\n");

    printf("RunInPainting in \nCPU time : %lf in average, and %lf in total\nIRL time : %lf in average, and %lf in total\nThe gain in time is of %.3lf%c \n", (double)(duraclock/cptsave), duraclock,(double)(duratime/cptsave), duratime,(duraclock-duratime)*100/duraclock,'%');
    
}