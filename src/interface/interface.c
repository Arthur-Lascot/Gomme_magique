#include <gtk/gtk.h>

//interface
typedef struct Inter
{
    GtkWindow* window;              // Main window
    GtkButton* Bs1;       
    GtkButton* Bs2;       
    GtkButton* Bs3;       
    GtkButton* Bse;       
    GtkButton* Bse_im;
    GtkImage* Gimage;
    int usless; //this is usless
} Inter;
//for the chose buttom
void on_fchose_image(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	//active another button
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs2),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs3),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bs1),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bse),TRUE);
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
		GdkPixbuf *pix = gdk_pixbuf_new_from_file(filename,NULL);
		if(pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,pix);
		}
    		g_free (filename);
  	}

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
}

void on_sim_2(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	printf("coucou\n");
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_sim_3(GtkButton *button, gpointer user_data)
{
	Inter* inter=user_data;
	printf("bip biiiiiiiip\n");
	if(button!=NULL)
	{
		inter->usless=1;
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
     if (gtk_builder_add_from_file(builder, "interface_n1.glade", &error) == 0)         
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
     
     GtkImage* Gimage = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
     
     Inter inter=
     {
	     .Bse=Bse,
	     .Bs3=Bs3,
	     .Bs2=Bs2,
	     .Bs1=Bs1,
	     .Bse_im=Bchose_image,
	     .Gimage=Gimage,
	     .usless=1
     };
     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &inter);
     g_signal_connect(Bchose_image, "clicked", 
		     G_CALLBACK(on_fchose_image), &inter);
     g_signal_connect(Bse, "clicked", G_CALLBACK(on_select), &inter);
     g_signal_connect(Bs3, "clicked", G_CALLBACK(on_sim_3), &inter);
     g_signal_connect(Bs2, "clicked", G_CALLBACK(on_sim_2), &inter);
     g_signal_connect(Bs1, "clicked", G_CALLBACK(on_sim_1), &inter);
	printf("coucou\n");
     gtk_main();
     return 1;                                                             
 } 
