#ifndef IN_PAINTING_H
#define IN_PAINTING_H

#define PSY_W 9

void is_valid(int pixel, int w, int h, int *ret);

double conf(int pixel, double *C, int w, int h);

double dist_psi(SDL_Surface *surface, int p, int q, int *map);

void update_border(int *map, int x, int y, int w, int h);

void copy(SDL_Surface *surface, double *C, int *map, int p, int q, double conf_p);

double data_term();

void inPainting(SDL_Surface *surface, int *map);

#endif