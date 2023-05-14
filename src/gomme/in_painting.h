#ifndef IN_PAINTING_H
#define IN_PAINTING_H

#define PSY_W 9
#define DEBUG 0

typedef struct {
    SDL_Surface *surface;
    int* map;
    double max_p;
    int qstart;
    int qend;
    double min;
    double min_d;
} dpsi_arg;

void is_valid(int pixel, int w, int h, int *ret);

double conf(int pixel, double *C, int w, int h);

void get_sobel(SDL_Surface *surface, int point, int *grad);

double data_term(SDL_Surface *surface, int p, int *map);

double dist_psi(SDL_Surface *surface, int p, int q, int *map);

void *dist_psi_threaded(void *arg);

void update_border(SDL_Surface *surface, int *map, int x, int y, int w, int h);

void copy(SDL_Surface *surface, double *C, int *map, int p, int q, double conf_p);

void inPainting(SDL_Surface *surface, int *map, int w, int h, double *C, int start);

void run_inPainting(SDL_Surface *surface, int *map);

#endif