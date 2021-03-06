#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include "misc.h"
#ifndef __USE_BSD
#define __USE_BSD
#endif
#include <math.h>

void open_url(const char *uri)
{
    gchar *data = g_strdup_printf("xdg-open '%s'", uri);
    g_spawn_command_line_async(data, NULL);
    g_free(data);
}

void open_email(const char *uri)
{
    gchar *data = g_strdup_printf("xdg-open 'mailto:%s'", uri);
    g_spawn_command_line_async(data, NULL);
    g_free(data);
}

UrlType url_type(const char *uri)
{
    /* check for http url */
    if(g_regex_match_simple(
                "^(rsync|http|ftp|https|scp|irc)://.*",
                uri, 
                G_REGEX_CASELESS,G_REGEX_MATCH_NOTEMPTY))
    {
        return URL_HTTP;
    }

    /* check for e-mail adres */
    else if (g_regex_match_simple(
                "^\\w+[\\.\\+\\_\\-\\a-z]*@+\\w+[\\.\\+\\_\\-\\a-z]*.{1}\\w{2,3}.?\\w{2,3}$",
                uri, 
                G_REGEX_CASELESS,G_REGEX_MATCH_NOTEMPTY))
    {
        return URL_EMAIL;
    }
    else
    {
        return URL_OTHER;
    }
}

/**
 * Code copied from gnome-screenshot
 */
#define BLUR_RADIUS    5
#define SHADOW_OFFSET  (BLUR_RADIUS * 4 / 5)
#define SHADOW_OPACITY 0.8

#define OUTLINE_RADIUS  1.2
#define OUTLINE_OFFSET  0
#define OUTLINE_OPACITY 1.0

#define dist(x0, y0, x1, y1) sqrt(((x0) - (x1))*((x0) - (x1)) + ((y0) - (y1))*((y0) - (y1)))

typedef struct {
  int size;
  double *data;
} ConvFilter;

static double
gaussian (double x, double y, double r)
{
    return ((1 / (2 * M_PI * r)) *
	    exp ((- (x * x + y * y)) / (2 * r * r)));
}

static ConvFilter *
create_blur_filter (int radius)
{
  ConvFilter *filter;
  int x, y;
  double sum;
  
  filter = g_new0 (ConvFilter, 1);
  filter->size = radius * 2 + 1;
  filter->data = g_new (double, filter->size * filter->size);

  sum = 0.0;
  
  for (y = 0 ; y < filter->size; y++)
    {
      for (x = 0 ; x < filter->size; x++)
	{
	  sum += filter->data[y * filter->size + x] = gaussian (x - (filter->size >> 1),
								y - (filter->size >> 1),
								radius);
	}
    }

  for (y = 0; y < filter->size; y++)
    {
      for (x = 0; x < filter->size; x++)
	{
	  filter->data[y * filter->size + x] /= sum;
	}
    }

  return filter;
  
}


static GdkPixbuf *
create_effect (GdkPixbuf *src, ConvFilter const *filter, int radius, int offset, double opacity)
{
    GdkPixbuf *dest;
    int x, y, i, j;
    int src_x, src_y;
    int suma;
    int dest_width, dest_height;
    int src_width, src_height;
    int src_rowstride, dest_rowstride;
    gboolean src_has_alpha;


    guchar *src_pixels, *dest_pixels;

    /* No source, nothing todo */
    if(!src) {
        return NULL;
    }

    src_has_alpha =  gdk_pixbuf_get_has_alpha (src);

    src_width = gdk_pixbuf_get_width (src);
    src_height = gdk_pixbuf_get_height (src);
    dest_width = src_width + 2 * radius + offset;
    dest_height = src_height + 2 * radius + offset;

    dest = gdk_pixbuf_new (gdk_pixbuf_get_colorspace (src),
            TRUE,
            gdk_pixbuf_get_bits_per_sample (src),
            dest_width, dest_height);

    gdk_pixbuf_fill (dest, 0);  

    src_pixels = gdk_pixbuf_get_pixels (src);
    src_rowstride = gdk_pixbuf_get_rowstride (src);

    dest_pixels = gdk_pixbuf_get_pixels (dest);
    dest_rowstride = gdk_pixbuf_get_rowstride (dest);

    for (y = 0; y < dest_height; y++)
    {
        for (x = 0; x < dest_width; x++)
        {
            suma = 0;

            src_x = x - radius;
            src_y = y - radius;

            /* We don't need to compute effect here, since this pixel will be 
             * discarded when compositing */
            if (src_x >= 0 && src_x < src_width &&
                    src_y >= 0 && src_y < src_height && 
                    (!src_has_alpha ||
                     src_pixels [src_y * src_rowstride + src_x * 4 + 3] == 0xFF))
                continue;

            for (i = 0; i < filter->size; i++)
            {
                for (j = 0; j < filter->size; j++)
                {
                    src_y = -(radius + offset) + y - (filter->size >> 1) + i;
                    src_x = -(radius + offset) + x - (filter->size >> 1) + j;

                    if (src_y < 0 || src_y >= src_height ||
                            src_x < 0 || src_x >= src_width)
                        continue;

                    suma += ( src_has_alpha ? 
                            src_pixels [src_y * src_rowstride + src_x * 4 + 3] :
                            0xFF ) * filter->data [i * filter->size + j];
                }
            }

            dest_pixels [y * dest_rowstride + x * 4 + 3] = CLAMP (suma * opacity, 0x00, 0xFF);
        }
    }

    return dest;
}

void
screenshot_add_shadow (GdkPixbuf **src)
{
    GdkPixbuf *dest;
    static ConvFilter *filter = NULL;

    /* If *src == NULL, return nothing to do 
    */
    if(!(*src))
        return;


    if (!filter)
        filter = create_blur_filter (BLUR_RADIUS);

    dest = create_effect (*src, filter, 
            BLUR_RADIUS, SHADOW_OFFSET, SHADOW_OPACITY);

    /* Failed to create effect */
    if (dest == NULL)
        return;

    gdk_pixbuf_composite (*src, dest,
            BLUR_RADIUS, BLUR_RADIUS,
            gdk_pixbuf_get_width (*src),
            gdk_pixbuf_get_height (*src),
            BLUR_RADIUS, BLUR_RADIUS, 1.0, 1.0,
            GDK_INTERP_NEAREST, 255);
    g_object_unref (*src);
    *src = dest;
}

void screenshot_add_border (GdkPixbuf **src)
{
    GdkPixbuf *pb = *src;
    int x,y,width,height;
    int pixel;
    int n_channels = 0;
    int rowstride = 0;
    guchar *pixels;

    if(!pb){
        return;
    }

    rowstride = gdk_pixbuf_get_rowstride(pb);	
    n_channels = gdk_pixbuf_get_n_channels(pb);
    width = gdk_pixbuf_get_width (pb);
    height = gdk_pixbuf_get_height (pb);
    pixels = gdk_pixbuf_get_pixels(pb);

    for(y=0;y<height;y++)
    {
        for(x=0;x<width;x++)
        {
            if(y == 0 || y == (height-1) || x == 0 || x == (width-1))
            {
                for(pixel=0; pixel < n_channels;pixel++)
                {
                    pixels[x*n_channels+y*rowstride+pixel] = 0;
                }
            }
        }
    }
}
GdkPixbuf * gdk_pixbuf_new_from_file_at_max_size(const char *uri, int size)
{
	GdkPixbuf *pb = NULL;
	gint width=0, height=0;
	if(gdk_pixbuf_get_file_info(uri, &width, &height))
	{
		if(width > size || height > size)
			pb = gdk_pixbuf_new_from_file_at_scale(uri, size,size,TRUE,NULL);
		else
			pb = gdk_pixbuf_new_from_file(uri, NULL);
	}
	return pb;
}

static void __update_preview_widget (GtkWidget *fc, GtkWidget *image)
{
	char *uri = gtk_file_chooser_get_preview_filename(GTK_FILE_CHOOSER(fc));
	if(uri)
	{
		GdkPixbuf *pb = NULL;
		gpointer data = g_object_get_data(G_OBJECT(fc), "size");
		int size = (data != NULL)? GPOINTER_TO_INT(data):16;
		pb = gdk_pixbuf_new_from_file_at_max_size(uri,size); 
		if(pb) {
			gtk_image_set_from_pixbuf(GTK_IMAGE(image), pb);
			gtk_file_chooser_set_preview_widget_active (GTK_FILE_CHOOSER(fc), TRUE);
		} else {
			gtk_file_chooser_set_preview_widget_active (GTK_FILE_CHOOSER(fc), FALSE);
        }
		g_free(uri);
	}
}

void file_chooser_enable_image_preview(GtkWidget *fc, int size)
{
	GtkWidget *image = gtk_image_new();
	gtk_file_chooser_set_use_preview_label(GTK_FILE_CHOOSER(fc), FALSE);
	gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(fc), image);
	g_object_set_data(G_OBJECT(fc), "size", GINT_TO_POINTER(size));
	g_signal_connect(G_OBJECT(fc), "update-preview", G_CALLBACK(__update_preview_widget), image);
}

