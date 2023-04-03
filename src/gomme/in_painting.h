#ifndef IN_PAINTING_H
#define IN_PAINTING_H

#define PSY_W 9

#define MATX {  1, 0, -1,
                2, 0, -2,
                1, 0, -1}

#define MATY {  1, 2, 1,
                0, 0, 0,
                -1, -2, -1}

void is_valid(int pixel, int w, int h, int *ret);

double conf(int pixel, double *C, int w, int h);

void get_sobel(SDL_Surface *surface, int point, int *grad);

double data_term(SDL_Surface *surface, int p, int *map);

double dist_psi(SDL_Surface *surface, int p, int q, int *map);

void update_border(int *map, int x, int y, int w, int h);

void copy(SDL_Surface *surface, double *C, int *map, int p, int q, double conf_p);

void inPainting(SDL_Surface *surface, int *map);

#endif