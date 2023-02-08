#include <err.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>

const int max_DIF=25;
const int DIFF=20;

// Updates the display.
//
// renderer: Renderer to draw on.
// texture: Texture that contains the image.
void draw(SDL_Renderer* renderer, SDL_Texture* texture)
{
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Event loop that calls the relevant event handler.
//
// renderer: Renderer to draw on.
// colored: Texture that contains the colored image.
// grayscale: Texture that contains the grayscale image.
void event_loop(SDL_Renderer* renderer, SDL_Texture* colored, SDL_Texture* grayscale)
{
    SDL_Event event;
    SDL_Texture* t = colored;
	draw(renderer,t);
    while (1)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            // If the "quit" button is pushed, ends the event loop.
            case SDL_QUIT:
                return;

            // If the window is resized, updates and redraws the diagonals.
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    draw(renderer, t);
                }
                break;
	case SDL_KEYDOWN:
		if(event.key.keysym.sym==SDLK_a)
		{
			if (t==colored)
			{
				t=grayscale;
				draw(renderer,t);
			}
			else
			{
				t=colored;
				draw(renderer,t);
			//	printf("changer\n");
			}	
		}
		if(event.key.keysym.sym==SDLK_LEFT)
		{
		
		}
        }
    }
}

Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format);
SDL_Surface* load_image(const char* path);

Uint32 pixel_to_black(Uint32 pixel_color, SDL_PixelFormat* format) 
	// convert a pixel on a black pixel
{
    Uint8 r, g, b;
	SDL_GetRGB(pixel_color, format, &r, &g, &b);
	int average= 0.0*r + 0.0*g + 0.0*b;
	return SDL_MapRGB(format, average,average ,average );
}

Uint32 copy_pixel(Uint32 pixel,SDL_PixelFormat* format)
	//deep copy of pixel
{

    	Uint8 r, g, b;
	SDL_GetRGB(pixel, format, &r, &g, &b);
	return SDL_MapRGB(format, r,g ,b );
}


// Loads an image in a surface.
// The format of the surface is SDL_PIXELFORMAT_RGB888.
//
// path: Path of the image.
SDL_Surface* load_image(const char* path)
{
    SDL_Surface* surface= IMG_Load(path);
    return SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888,0);
}

// Converts a colored pixel into grayscale.
//
// pixel_color: Color of the pixel to convert in the RGB format.
// format: Format of the pixel used by the surface.
Uint32 pixel_to_grayscale(Uint32 pixel_color, SDL_PixelFormat* format)
{
    Uint8 r, g, b;
	SDL_GetRGB(pixel_color, format, &r, &g, &b);
	int average= 0.3*r + 0.59*g + 0.11*b;
	return SDL_MapRGB(format, average,average ,average );
}

void surface_to_grayscale(SDL_Surface* surface)
{
    Uint32* pixels = surface->pixels;
    int len = surface->w * surface->h;
    SDL_PixelFormat* format = surface->format;
    SDL_LockSurface(surface);
    for(int i=0;i<len;i++)
    {
	    pixels[i] = pixel_to_grayscale(pixels[i],format);
    }
    SDL_UnlockSurface(surface);
}

void rec_to_image(Uint32* pixels,SDL_PixelFormat* format,size_t i,size_t j,
		char tab[], size_t h,size_t w,Uint8 r,Uint8 g,Uint8 b)
{
	int max_diff=100;
	//printf("i:%li  j:%li  i*j:%li, max:%li max_i: %li max_j:%li\n",i,j,j+i*h,h*w,w,h);
	if(i<w &&j<h && tab[j+i*h]==0)
	{
		Uint8 r1, g1, b1;
		SDL_GetRGB(pixels[j+i*h], format, &r1, &g1, &b1);
		if(max_diff>=abs(r-r1)+abs(b-b1)+abs(g-g1))
		{
			tab[j+i*h]=1;
			pixels[j+i*h]=SDL_MapRGB(format, r,g ,b);
			//printf("i:%li  j:%li  i*j:%li, max:%li\n",i,j,j+i*h,h*w);
			if(j>0)
			{
				rec_to_image(pixels,format,i,j-1,tab,h,w,r,g,b);
			}
			if(j<h-1)
			{
				rec_to_image(pixels,format,i,j+1,tab,h,w,r,g,b);
			}
			if(i>0)
			{
				rec_to_image(pixels,format,i-1,j,tab,h,w,r,g,b);
			}
			if(i<w-1)
			{
				rec_to_image(pixels,format,i+1,j,tab,h,w,r,g,b);
			}
		}
	}
}

void surface_to_image_rec(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels;
	size_t w = surface->w;
       	size_t h=surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	char tab[h*w];
	for (size_t i=0;i<h*w;i++)
	{
		tab[i]=0;
	}	
	//printf("max_h :%li  max_w:%li\n",h,w);
	for(size_t i =0;i<w;i++)
	{
		for(size_t j =0;j<h;j++)
		{
			if(tab[j+i*h]==0)
			{
				Uint8 r, g, b;
				SDL_GetRGB(pixels[j+i*h], format, &r, &g, &b);
				rec_to_image(pixels,format,i,j,tab,h,w,r,g,b);
			}
		}
	}
	SDL_UnlockSurface(surface);
}

int mat_to_image(Uint32* pixels,SDL_PixelFormat* format,size_t w,size_t h,int max_diff,int diff)
{
	int bouboul=1;
	for(size_t i =0;i<h;i++)
	{
		for(size_t j =0;j<w;j++)
		{
			Uint8 r, g, b;
			SDL_GetRGB(pixels[i*w+j], format, &r, &g, &b);
			int moy=1;
			int rf=r;
			int gf=g;
			int bf=b;

			int rb=r;
			int gb=g;
			int bb=b;
			if(i>0)
			{
				SDL_GetRGB(pixels[(i-1)*w+j]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
				if(j>0)
				{
					SDL_GetRGB(pixels[(i-1)*w+j-1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}

				}
				if(j<w)
				{
					SDL_GetRGB(pixels[(i-1)*w+j+1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}
				}

			}
			if(i<h)
			{
				SDL_GetRGB(pixels[(i+1)*w+j]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
				if(j>0)
				{
					SDL_GetRGB(pixels[(i+1)*w+j-1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}

				}
				if(j<w)
				{
					SDL_GetRGB(pixels[(i+1)*w+j+1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}
				}
			}
			if(j>0)
			{
				SDL_GetRGB(pixels[(i)*w+j-1]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
			}
			if(j<w)
			{
				SDL_GetRGB(pixels[(i)*w+j+1]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
			}
			if(moy!=0)
			{
				/*rf=rf/moy;
				gf=gf/moy;
				bf=bf/moy;*/
				if(rf/moy+rf%moy<255)
				{
					rf=rf/moy+rf%moy%2;
				}
				else
				{
					rf=rf/moy;
				}
				if(bf/moy+bf%moy<255)
				{
					bf=bf/moy+bf%moy%2;
				}
				else
				{
					bf=bf/moy;
				}
				if(gf/moy+gf%moy<255)
				{
					gf=gf/moy+gf%moy%2;
				}
				else
				{
					gf=gf/moy;
				}

				if(abs(rb-rf)+abs(gb-gf)+abs(bb-bf)>diff)
				{
					bouboul=0;
				}
			}
			if(i>0)
			{
				SDL_GetRGB(pixels[(i-1)*w+j]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					pixels[(i-1)*w+j]=SDL_MapRGB(format, rf,gf ,bf);
				}
				if(j>0)
				{
					SDL_GetRGB(pixels[(i-1)*w+j-1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						pixels[(i-1)*w+j-1]=SDL_MapRGB(format, rf,gf ,bf);
					}

				}
				if(j<w)
				{
					SDL_GetRGB(pixels[(i-1)*w+j+1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						pixels[(i-1)*w+j+1]=SDL_MapRGB(format, rf,gf ,bf);
					}
				}

			}
			if(i<h)
			{
				SDL_GetRGB(pixels[(i+1)*w+j]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					pixels[(i+1)*w+j]=SDL_MapRGB(format, rf,gf ,bf);
				}
				if(j>0)
				{
					SDL_GetRGB(pixels[(i+1)*w+j-1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						pixels[(i+1)*w+j-1]=SDL_MapRGB(format, rf,gf ,bf);
					}

				}
				if(j<w)
				{
					SDL_GetRGB(pixels[(i+1)*w+j+1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						pixels[(i+1)*w+j+1]=SDL_MapRGB(format, rf,gf ,bf);
					}
				}
			}
			if(j>0)
			{
				SDL_GetRGB(pixels[(i)*w+j-1]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					pixels[i*w+j-1]=SDL_MapRGB(format, rf,gf ,bf);
				}
			}
			if(j<w)
			{
				SDL_GetRGB(pixels[(i)*w+j+1]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					pixels[i*w+j+1]=SDL_MapRGB(format, rf,gf ,bf);
				}
			}
			pixels[i*w+j]=SDL_MapRGB(format, rf,gf ,bf);
		}
	}
	return bouboul;
}

int mat_to_image_fl(Uint32* pixels,SDL_PixelFormat* format,size_t w,size_t h,int max_diff)
{
	int bouboul=1;
	for(size_t i =0;i<h;i++)
	{
		for(size_t j =0;j<w;j++)
		{
			Uint8 r, g, b;
			SDL_GetRGB(pixels[i*w+j], format, &r, &g, &b);
			int moy=1;
			int rf=r;
			int gf=g;
			int bf=b;

			int rb=r;
			int gb=g;
			int bb=b;
			if(i>0)
			{
				SDL_GetRGB(pixels[(i-1)*w+j]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
				if(j>0)
				{
					SDL_GetRGB(pixels[(i-1)*w+j-1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}

				}
				if(j<w)
				{
					SDL_GetRGB(pixels[(i-1)*w+j+1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}
				}

			}
			if(i<h)
			{
				SDL_GetRGB(pixels[(i+1)*w+j]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
				if(j>0)
				{
					SDL_GetRGB(pixels[(i+1)*w+j-1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}

				}
				if(j<w)
				{
					SDL_GetRGB(pixels[(i+1)*w+j+1]
							, format, &r, &g, &b);
					if(abs((rb+gb+bb)-(r+g+b))<max_diff)
					{
						moy+=1;
						rf+=r;
						gf+=g;
						bf+=b;
					}
				}
			}
			if(j>0)
			{
				SDL_GetRGB(pixels[(i)*w+j-1]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
			}
			if(j<w)
			{
				SDL_GetRGB(pixels[(i)*w+j+1]
						, format, &r, &g, &b);
				if(abs((rb+gb+bb)-(r+g+b))<max_diff)
				{
					moy+=1;
					rf+=r;
					gf+=g;
					bf+=b;
				}
			}
			if(moy!=0)
			{
				/*rf=rf/moy;
				gf=gf/moy;
				bf=bf/moy;*/
				if(rf/moy+rf%moy<255)
				{
					rf=rf/moy+rf%moy%2;
				}
				else
				{
					rf=rf/moy;
				}
				if(bf/moy+bf%moy<255)
				{
					bf=bf/moy+bf%moy%2;
				}
				else
				{
					bf=bf/moy;
				}
				if(gf/moy+gf%moy<255)
				{
					gf=gf/moy+gf%moy%2;
				}
				else
				{
					gf=gf/moy;
				}
			}
			pixels[i*w+j]=SDL_MapRGB(format, rf,gf ,bf);
		}
	}
	return bouboul-1;
}

void surface_to_image_mat(SDL_Surface* surface)
{
	Uint32* pixels = surface->pixels;
	size_t w = surface->w;
       	size_t h=surface->h;
	SDL_PixelFormat* format = surface->format;
	SDL_LockSurface(surface);
	/*int i=0;
	while(i<100)
	{
		i++;
		if(mat_to_image_fl(pixels,format,w,h,25))
			break;
	}*/
	while(356)
	{
		mat_to_image_fl(pixels,format,w,h,25);
		if(mat_to_image(pixels,format,w,h,max_DIF,DIFF))
		{
			break;
		}
	}
	while(356)
	{
		if(mat_to_image(pixels,format,w,h,10,9))
			break;
	}
	SDL_UnlockSurface(surface);
}


int main(int argc, char** argv)
{
    // Checks the number of arguments.
    if (argc  >3)
        errx(EXIT_FAILURE, "Usage: image-file");

    // - Initialize the SDL.
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a window.
    SDL_Window* window = 
	    SDL_CreateWindow("My picture", 0, 0, 640, 640,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a renderer.
    SDL_Renderer* renderer = 
	    SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        errx(EXIT_FAILURE, "%s", SDL_GetError());
    
    // - Create a surface from the colored image.
   SDL_Surface* surface= load_image(argv[1]); 
    // - Resize the window according to the size of the image.
    //SDL_SetWindowSize(renderer, surface->w,surface->h);
   
    // - Create a texture from the colored surface.
    SDL_Texture* texture= SDL_CreateTextureFromSurface(renderer,surface);
    
   surface_to_image_mat(surface); 
    // - Create a new texture from the grayscale surface.
    SDL_Texture* textureg= SDL_CreateTextureFromSurface(renderer,surface);

    // - Free the surface.
    SDL_FreeSurface( surface);

    // - Dispatch the events.
	event_loop(renderer, texture, textureg);
   
    // - Destroy the objects.
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
