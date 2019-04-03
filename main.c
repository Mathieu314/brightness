#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

void OnDestroy(GtkWidget *pWidget, gpointer pData);
void OnScrollbarChange(GtkWidget *pWidget, gpointer data);

int main(int argc, char **argv) {
	FILE *fichier = NULL;
	char value[4];
	fichier = fopen("/sys/class/backlight/intel_backlight/brightness", "r");
	fscanf(fichier, "%s", value);
	fclose(fichier);
	int nb = atoi(value);
	// Debug
	//printf("v%s", value);
	/* Initialisation de gtk */
	gtk_init(&argc, &argv);
	/* declaration et initialisation des widgets */
	GtkWidget *pMainVBox;
	GtkWidget *pWindow;
	GtkWidget *pFrame;
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *pLabel;
	GtkWidget *pScrollbar;
	GtkObject *Adjust;
	GtkWidget *pBrightnessBox;

	pMainVBox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(pWindow), pMainVBox);

	pFrame = gtk_frame_new("Set brightness :");
	gtk_box_pack_start(GTK_BOX(pMainVBox), pFrame, FALSE, FALSE, 0);
	pBrightnessBox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(pFrame), pBrightnessBox);
	pLabel = gtk_label_new(value);
	gtk_box_pack_start(GTK_BOX(pBrightnessBox), pLabel, FALSE, FALSE, 0);
	Adjust = gtk_adjustment_new(nb, 100, 4882, 1, 10, 1);
	pScrollbar = gtk_hscrollbar_new(GTK_ADJUSTMENT(Adjust));
	gtk_box_pack_start(GTK_BOX(pBrightnessBox), pScrollbar, TRUE, TRUE, 0);


	/* Titre */
	gtk_window_set_title(GTK_WINDOW(pWindow), "Brightness");
	/* Taille */
	gtk_window_set_default_size(GTK_WINDOW(pWindow), 500, 200);
	/* connexion du signal "destroy" */
	g_signal_connect(G_OBJECT(pWindow), "destroy", G_CALLBACK(OnDestroy), NULL);
	/* connexion du signal "value-changed" */
	g_signal_connect(G_OBJECT(pScrollbar), "value-changed", G_CALLBACK(OnScrollbarChange), (GtkWidget*)pLabel);
	/* affichage */
	gtk_widget_show_all(pWindow);
	
	/* demarrage de la boucle evenementielle */	
	gtk_main();
	return 0;
}

void OnDestroy(GtkWidget *pWidget, gpointer pData) {
	gtk_main_quit();
}

void OnScrollbarChange(GtkWidget *pWidget, gpointer data) {
	FILE *fichier = NULL;
	gchar* sLabel;
	gint iValue;
	iValue = gtk_range_get_value(GTK_RANGE(pWidget));
	sLabel = g_strdup_printf("%d", iValue);
	gtk_label_set_text(GTK_LABEL(data), sLabel);
	fichier = fopen("/sys/class/backlight/intel_backlight/brightness", "w+");
	fprintf(fichier, "%d", iValue);
	fclose(fichier);
	g_free(sLabel);
}
