#include <gtk/gtk.h>
#include "../change_image/image_D.h"

//interface
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
    SDL_Surface* surface;
    SDL_Surface* surface1;
    SDL_Surface* surface2;
    char image;
    int usless; //this is usless
} Inter;

void create_image(char* filename,Inter* inter)
{
    SDL_Surface* surface= IMG_Load(filename);
    surface=SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGB888,0);
    surface=resize(surface,400,530);
    SDL_SaveBMP(surface, "image.png");
    SDL_Surface* surface1=surface_to_image_mat(surface);
    SDL_SaveBMP(surface1, "image1.png");
    SDL_Surface* surface2=surface_to_image_mat2(surface);
    SDL_SaveBMP(surface2, "image2.png");
    inter->surface=surface;
    inter->surface1=surface1;
    inter->surface2=surface2;
    SDL_FreeSurface( surface);
    SDL_FreeSurface( surface1);
    SDL_FreeSurface( surface2);
}

//for the chose buttom
void on_fchose_image(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	//active another button
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
	GdkPixbuf *pix = gdk_pixbuf_new_from_file("image2.png",NULL);
	if(pix!=NULL)
	{
		gtk_image_set_from_pixbuf(inter->Gimage,pix);
	}
}
void on_sim_3(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
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
		if(inter->image==0)
			surface=inter->surface;
		if(inter->image==1)
			surface=inter->surface1;
		if(inter->image==2)
			surface=inter->surface2;
		printf("c'est ici que tu met ton code avec inter->LP la liste et inter->len le nombre déléments\n");	
	}
}

void on_select(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
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
	printf("%i %i\n",x1,y1);
	inter->LP[inter->len*2]=x1;
	inter->LP[inter->len*2+1]=y1;
	inter->len++;
}

// Main function.                                                               
int main ()                                               
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
         return 1;                                                               
     }                                                                           
     // Gets the widgets.                                                        
     GtkWindow* window = 
	     GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.inter"));
    
     GtkButton* Bchose_image = 
	     GTK_BUTTON(gtk_builder_get_object(builder, "se_im"));
      
     GtkButton* Bs1 = GTK_BUTTON(gtk_builder_get_object(builder, "si1"));
     
     GtkButton* Bs2 = GTK_BUTTON(gtk_builder_get_object(builder, "si2"));
     
     GtkButton* Bs3 = GTK_BUTTON(gtk_builder_get_object(builder, "si3"));
      
     GtkButton* Bse = GTK_BUTTON(gtk_builder_get_object(builder, "se"));
     
     GtkButton* Bsi = GTK_BUTTON(gtk_builder_get_object(builder, "boutton_image"));
     
     GtkImage* Gimage = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
     gtk_widget_set_focus_on_click(GTK_WIDGET(Bsi),FALSE); 
	gtk_widget_set_sensitive(GTK_WIDGET(Bs2),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(Bs1),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(Bse),FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(Bs3),FALSE);
     
     int* LP=malloc(sizeof(int)*1024);
     size_t len=0;
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
	     .image=0
     };
     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &inter);
     g_signal_connect(Bchose_image, "clicked", 
		     G_CALLBACK(on_fchose_image), &inter);
     g_signal_connect(Bse, "clicked", G_CALLBACK(on_select), &inter);
     g_signal_connect(Bs2, "clicked", G_CALLBACK(on_sim_2), &inter);
     g_signal_connect(Bs1, "clicked", G_CALLBACK(on_sim_1), &inter);
     g_signal_connect(Bs3, "clicked", G_CALLBACK(on_sim_3), &inter);
     g_signal_connect(Bsi, "clicked", G_CALLBACK(on_click), &inter);
     gtk_main();
     return 1;                                                             
 } 
