
#include <gtk/gtk.h>
#include "interface/interface.h"

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