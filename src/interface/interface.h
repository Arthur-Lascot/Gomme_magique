#ifndef INTERFACE_H_
#define INTERFACE_H_
#include <gtk/gtk.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct Inter
{
    GtkWindow* window;              // Main window
    GtkButton* Bs1;       
    GtkButton* Bs2;      
    GtkButton* Bs3;
    GtkButton* Bse;       
    GtkButton* Bse_im;
    GtkButton* Bsi;
    GtkImage* Gimage;
    int* LP;
    size_t len;
    SDL_Surface* surfaceOri;
    SDL_Surface* surface;
    SDL_Surface* surface1;
    SDL_Surface* surface2;
    char image;
    int usless; //this is usless
} Inter;

void create_image(char* filename,Inter* inter);
void on_fchose_image(GtkButton *button, gpointer user_data);
void on_sim_1(GtkButton *button, gpointer user_data);
void on_sim_2(GtkButton *button, gpointer user_data);
void on_sim_3(GtkButton *button, gpointer user_data);
void on_select(GtkButton *button, gpointer user_data);
void on_click(GtkButton *button, gpointer user_data);
#endif