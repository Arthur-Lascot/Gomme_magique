#include <gtk/gtk.h>
#include "../change_image/image_D.h"
#include "../STP/selection_to_pixel.h"
#include "interface.h"

//create image
void create_image(char* filename,Inter* inter)
{
	SDL_Surface* surfaceOri= IMG_Load(filename);
	SDL_Surface *surface = surfaceOri;
	surface = SDL_ConvertSurfaceFormat(surfaceOri, SDL_PIXELFORMAT_RGB888, 0);
	surface=resize(surface,inter->d1,inter->d2);
	SDL_SaveBMP(surface, "image.png");
	inter->surfaceOri = surfaceOri;
	if(inter->surface!=NULL)
		SDL_FreeSurface(inter->surface);
	if(inter->surface1!=NULL)
		SDL_FreeSurface(inter->surface1);
	if(inter->surface2!=NULL)
		SDL_FreeSurface(inter->surface2);
	inter->surface=surface;
	inter->surface1=NULL;
	inter->surface2=NULL;
}

//for the chose buttom
void on_fchose_image(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	inter->LP = malloc(20 * sizeof(int));
	inter->len = 0;
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
		inter->pix = gdk_pixbuf_new_from_file("image.png",NULL);
		if(inter->pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,inter->pix);
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
// launch first simplication
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
// launch segond simplification
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

// this is final function for saving points 
void on_sim_3(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	printf("coucou\n");
	if(button!=NULL)
	{
		inter->usless=1;
	}
	// set sensibiliti
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
		inter->pix = gdk_pixbuf_new_from_file("image4.png",NULL);
		if(inter->pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,inter->pix);
		}
		printf("Test validé");
		free(map);
		SDL_FreeSurface(surface);	
	}
}
// launch saving of points
void on_select(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
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
// take the location of click
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
	/*printf("x1 = %d\n y1 = %d\n", (inter->LP[inter->len * 2]), 
		(inter->LP[inter->len * 2 + 1]));*/
	inter->len=(inter->len+1)%510;

}
// launch setting windows
void Loption(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	if(button!=NULL)
	{
		inter->usless=1;
	}
	gtk_widget_set_visible(GTK_WIDGET(inter->W_Option),TRUE);
}
// hidde setting windows
void invisible(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	if(button!=NULL)
	{
		inter->usless=1;
	}
	// take size of image and he change this
	double d1;
	double d2;
	d1=gtk_range_get_value(GTK_RANGE(inter->W_im));
	d2=gtk_range_get_value(GTK_RANGE(inter->H_im));
	gtk_widget_set_visible(GTK_WIDGET(inter->W_Option),FALSE);
	if(inter->surface!=NULL)
		SDL_FreeSurface(inter->surface);
	if(inter->surface1!=NULL)
		SDL_FreeSurface(inter->surface1);
	if(inter->surface2!=NULL)
		SDL_FreeSurface(inter->surface2);
	if (FALSE==gtk_toggle_button_get_active(inter->CB_size))
	{
		d1=677;
		d2=285;
	}
	inter->d1=d1;
	inter->d2=d2;
	if (inter->surfaceOri!=NULL)
	{
		SDL_Surface *surface = inter->surfaceOri;
		surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888, 0);
		surface=resize(surface,(int)d1,(int)d2);
		inter->surface=surface;
		inter->surface1=NULL;
		inter->surface2=NULL;
		SDL_SaveBMP(surface, "image.png");
		GdkPixbuf *pix = gdk_pixbuf_new_from_file("image.png",NULL);
		if(pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,pix);
		}
	}
	gtk_widget_set_size_request(GTK_WIDGET(inter->Bsi),(gint)d1,(gint)d2);
}
// create interface
void init_interface()
{                                                                               
	// Initializes GTK.                                                         
	gtk_init(NULL, NULL);                                                       

	// Loads the UI description and builds the UI.                              
	// (Exits if an error occurs.)                                              
	GtkBuilder* builder = gtk_builder_new();                                    
	GError* error = NULL;                                                       
	if (gtk_builder_add_from_file(builder, "src/interface/interface_n1.glade", &error) == 0)         
	{                                                                           
		g_printerr("Error loading file: %s\n", error->message);                 
		g_clear_error(&error);                                                                                                                
	}                                                                           
	// Gets the widgets.                                                        
	GtkWindow* window = 
		GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.inter"));
	GtkWindow* W_Option=
		GTK_WINDOW(gtk_builder_get_object
				(builder, "org.gtk.option"));

	GtkButton* Bchose_image = 
		GTK_BUTTON(gtk_builder_get_object(builder, "se_im"));

	GtkButton* Bs1 = GTK_BUTTON(gtk_builder_get_object(builder, "si1"));

	GtkButton* Bs2 = GTK_BUTTON(gtk_builder_get_object(builder, "si2"));

	GtkButton* Bs3 = GTK_BUTTON(gtk_builder_get_object(builder, "si3"));

	GtkButton* Bse = GTK_BUTTON(gtk_builder_get_object(builder, "se"));

	GtkButton* Bsi = GTK_BUTTON(gtk_builder_get_object(builder, "boutton_image"));

	GtkButton* Opt = GTK_BUTTON(gtk_builder_get_object(builder, "option"));
	GtkButton* close = GTK_BUTTON(gtk_builder_get_object(builder, "close"));

	GtkImage* Gimage = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
	GtkToggleButton* CB_size = GTK_TOGGLE_BUTTON(gtk_builder_get_object(builder, "CB_size"));
	GtkScale* W_im = GTK_SCALE(gtk_builder_get_object(builder, "W_im"));
	GtkScale* H_im = GTK_SCALE(gtk_builder_get_object(builder, "H_im"));
	GtkContainer* box_option= GTK_CONTAINER(gtk_builder_get_object(builder, "box_option"));
	gtk_widget_set_focus_on_click(GTK_WIDGET(Bsi),FALSE); 
	gtk_widget_set_sensitive(GTK_WIDGET(Bs2),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(Bs1),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(Bse),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(Bs3),FALSE);

	int* LP=malloc(sizeof(int)*1024);
	size_t len=0;
	//set a part of inter
	Inter inter=
	{
		.Bse=Bse,
		.Bs2=Bs2,
		.Bs1=Bs1,
		.Bs3=Bs3,
		.Bse_im=Bchose_image,
		.Gimage=Gimage,
		.usless=1,
		.Bsi=Bsi,
		.LP=LP,
		.len=len,
		.image=0,
		.Opt=Opt,
		.builder=builder,
		.W_Option=W_Option,
		.CB_size = CB_size,
		.W_im=W_im,
		.H_im=H_im,
		.box_option=box_option,
		.close=close,
		.d1=677,
		.d2=285,
	};
	// set button target
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &inter);
	g_signal_connect(Bchose_image, "clicked", 
			G_CALLBACK(on_fchose_image), &inter);
	g_signal_connect(Bse, "clicked", G_CALLBACK(on_select), &inter);
	g_signal_connect(Bs2, "clicked", G_CALLBACK(on_sim_2), &inter);
	g_signal_connect(Bs1, "clicked", G_CALLBACK(on_sim_1), &inter);
	g_signal_connect(Bs3, "clicked", G_CALLBACK(on_sim_3), &inter);
	g_signal_connect(Bsi, "clicked", G_CALLBACK(on_click), &inter);
	g_signal_connect(Opt, "clicked", G_CALLBACK(Loption), &inter);
	g_signal_connect(close, "clicked", G_CALLBACK(invisible), &inter);
	gtk_main();                                                             
} 
