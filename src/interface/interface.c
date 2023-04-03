#include <gtk/gtk.h>
#include "../change_image/image_D.h"
#include "../STP/selection_to_pixel.h"
#include "interface.h"
#include "../gomme/in_painting.h"
#include "../STP/tools.h"

void create_image(char* filename,Inter* inter)
{
	SDL_Surface *surfaceOri;
	SDL_Surface *surface;
	inter->surfaceOri = NULL;
	inter->surface = NULL;
	inter->surface1 = NULL;
	inter->surface2 = NULL;

	if (inter->surfaceOri != NULL)
		SDL_FreeSurface(inter->surfaceOri);
    if(inter->surface!=NULL)
	    SDL_FreeSurface(inter->surface);
    if(inter->surface1!=NULL)
	    SDL_FreeSurface(inter->surface1);
    if(inter->surface2!=NULL)
	    SDL_FreeSurface(inter->surface2);
	
    surfaceOri= IMG_Load(filename);
	surface = surfaceOri;
	//surface = SDL_ConvertSurfaceFormat(surfaceOri, SDL_PIXELFORMAT_RGB888, 0);
	SDL_SaveBMP(surface, "image.png");
	// surface=resize(surface,400,530);
	
	inter->surfaceOri = surfaceOri;
    inter->surface=surface;
    inter->surface1=NULL;
    inter->surface2=NULL;
}

//for the chose button
void on_fchose_image(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	// active another button
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs2),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs1),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bse),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs3),TRUE);
	//creat dialog windows
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
                                      NULL,
                                      action,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
  	{
    		char *filename;
    		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    		filename = gtk_file_chooser_get_filename (chooser);
		create_image(filename,inter);
		GdkPixbuf *pix = gdk_pixbuf_new_from_file("image.png",NULL);
		if(pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,pix);
		}
    		g_free (filename);
  	}
	inter->image=0;

	gtk_widget_destroy (dialog);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_sim_1(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	printf("coucou1\n");
	if(button!=NULL)
	{
		inter->usless=1;
	}
	if(inter->surface1==NULL)
	{
    		SDL_Surface* surface1=surface_to_image_mat(inter->surface);
   		SDL_SaveBMP(surface1, "image1.png");
    		inter->surface1=surface1;
	}
	GdkPixbuf *pix = gdk_pixbuf_new_from_file("image1.png",NULL);
	inter->image=1;
	if(pix!=NULL)
	{
		gtk_image_set_from_pixbuf(inter->Gimage,pix);
	}
}

void on_sim_2(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	printf("coucou\n");
	inter->image=2;
	if(button!=NULL)
	{
		inter->usless=1;
	}
	if(inter->surface2==NULL)
	{
    		SDL_Surface* surface2=surface_to_image_mat2(inter->surface);
    		SDL_SaveBMP(surface2, "image2.png");
    		inter->surface2=surface2;
	}
	GdkPixbuf *pix = gdk_pixbuf_new_from_file("image2.png",NULL);
	if(pix!=NULL)
	{
		gtk_image_set_from_pixbuf(inter->Gimage,pix);
	}
}
void on_sim_3(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	
	printf("coucou\n");
	if(button!=NULL)
	{
		inter->usless=1;
	}
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs2),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs1),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bse),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bse_im),TRUE);
	gtk_widget_set_focus_on_click(GTK_WIDGET(inter->Bsi),FALSE);
	gtk_widget_set_visible(GTK_WIDGET(inter->Bsi),FALSE);
	if (inter->len>=3)
	{
		SDL_Surface* surface;
		surface = inter->surfaceOri;
		printf("c'est ici que tu met ton code avec inter->LP la liste et inter->len le nombre déléments\n");
		int* map = drawBorder(inter->LP, inter->len, surface);
		fillPoly(surface, map);
		drawSide(surface, map);
		SDL_SaveBMP(surface, "image4.png");
		GdkPixbuf *pix = gdk_pixbuf_new_from_file("image4.png",NULL);
		if(pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,pix);
		}
		printf("Lancement de run_inPainting...\n");
		run_inPainting(surface, map);
		printf("InPainting fini, sauvegarde de l'image...\n");
		SDL_SaveBMP(surface, "image_inPainting.png");
		pix = gdk_pixbuf_new_from_file("image_inPainting.png",NULL);
		if(pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,pix);
		}
		printf("Test validé\n");
		free(map);	
	}
}

void on_select(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	if(inter->LP!=NULL)
		free(inter->LP);
	inter->LP = malloc(20 * sizeof(int));
	inter->len = 0;
	printf("toutoutou\n");
	if(button!=NULL)
	{
		inter->usless=1;
	}
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs2),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs1),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bse),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bse_im),FALSE);
	gtk_widget_set_focus_on_click(GTK_WIDGET(inter->Bsi),TRUE);
	gtk_widget_set_visible(GTK_WIDGET(inter->Bsi),TRUE);
	inter->len=0;
}
void on_click(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	if(button!=NULL)
	{
		inter->usless=1;
	}
	int x1,y1;
	gtk_widget_get_pointer(GTK_WIDGET(inter->Gimage),&x1,&y1);
	//printf("%i %i\n",x1,y1);
	inter->LP[inter->len*2]=x1;
	inter->LP[inter->len*2+1]=y1;
	//printf("x1 = %d\n y1 = %d\n", (inter->LP[inter->len * 2]), 
	//	(inter->LP[inter->len * 2 + 1]));
	inter->len++;
	//inter->LP = realloc(inter->LP, ((inter->len)*2 + 2) * sizeof(int));
	
}
