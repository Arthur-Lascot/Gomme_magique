#ifndef IMAGE_D_H_
#define IMAGE_D_H_
#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
SDL_Surface* load_image(const char* path);
Uint32 pixel_to_black(Uint32 pixel_color, SDL_PixelFormat* format);
Uint32 copy_pixel(Uint32 pixel,SDL_PixelFormat* format);
void rec_to_image(Uint32* pixels,SDL_PixelFormat* format,size_t i,size_t j,
		char tab[], size_t h,size_t w,Uint8 r,Uint8 g,Uint8 b);
void surface_to_image_rec(SDL_Surface* surface);
int mat_to_image(Uint32* pixels,SDL_PixelFormat* format,size_t w,size_t h,int max_diff,int diff);
int mat_to_image_fl(Uint32* pixels,SDL_PixelFormat* format,size_t w,size_t h,int max_diff);
void copy_su(Uint32* pixels,Uint32* pc,SDL_PixelFormat* format,size_t w,size_t h);
SDL_Surface* surface_to_image_mat(SDL_Surface* surface);
SDL_Surface* surface_to_image_mat2(SDL_Surface* surface);
void tri(unsigned int *tab,size_t el);
void set_tab(unsigned int *tabr,unsigned int *tabb,unsigned int *tabg,size_t max,Uint32* pixels,SDL_PixelFormat* format);
void set_list(unsigned int *tab,size_t len,unsigned int *tabf,int n);
void set_pixel(unsigned int *tabr,unsigned int *tabb,unsigned int *tabg,size_t max,Uint32* pixels,SDL_PixelFormat* format,size_t d,int n,Uint32* pixcopy);
SDL_Surface* surface_to_image_ave(SDL_Surface* surface);
size_t get_index(size_t x, size_t y, size_t maxx);                       
SDL_Surface* resize(SDL_Surface* surface, size_t newx, size_t newy);       
#endif
