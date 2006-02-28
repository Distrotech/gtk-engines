#include <gtk/gtk.h>
#include <math.h>
#include <string.h>

/* GTK 2.2 compatibility */
#ifndef GTK_IS_COMBO_BOX_ENTRY
	#define GTK_IS_COMBO_BOX_ENTRY(x) 0
#endif
#ifndef GTK_IS_COMBO_BOX
	#define GTK_IS_COMBO_BOX(x) 0
#endif

#define RADIO_SIZE 13
#define CHECK_SIZE 13

GtkTextDirection
cl_get_direction (GtkWidget *widget);

GtkTextDirection
cl_get_parent_direction (GtkWidget *widget);

GtkProgressBarOrientation
cl_get_progress_bar_orientation (GtkWidget *widget);

GdkPixbuf *
cl_generate_bit (unsigned char alpha[],
              GdkColor *color,
              double mult);

GdkPixbuf *
cl_colorize_bit (unsigned char *bit,
              unsigned char *alpha,
              GdkColor  *new_color);

GdkPixmap *
cl_pixbuf_to_pixmap (GtkStyle  *style,
                  GdkPixbuf *pixbuf,
                  GdkScreen *screen);

gboolean
cl_sanitize_size (GdkWindow      *window,
               gint           *width,
               gint           *height);

void
cl_rgb_to_hls (gdouble *r,
            gdouble *g,
            gdouble *b);

void
cl_hls_to_rgb (gdouble *h,
            gdouble *l,
            gdouble *s);

void
cl_shade (GdkColor * a, GdkColor * b, float k);

void
cl_draw_hgradient (GdkDrawable *drawable, GdkGC *gc, GtkStyle *style,
                int x, int y, int width, int height,
                GdkColor *top_color, GdkColor *bottom_color);

void
cl_draw_vgradient (GdkDrawable *drawable, GdkGC *gc, GtkStyle *style,
                int x, int y, int width, int height,
                GdkColor *left_color, GdkColor *right_color);

void
cl_arrow_draw_hline (GdkWindow     *window,
                  GdkGC         *gc,
                  int            x1,
                  int            x2,
                  int            y,
                  gboolean       last);

void
cl_arrow_draw_vline (GdkWindow     *window,
                  GdkGC         *gc,
                  int            y1,
                  int            y2,
                  int            x,
                  gboolean       last);

void
cl_draw_arrow (GdkWindow     *window,
            GdkGC         *gc,
            GdkRectangle  *area,
            GtkArrowType   arrow_type,
            gint           x,
            gint           y,
            gint           width,
            gint           height);

void
cl_calculate_arrow_geometry (GtkArrowType  arrow_type,
                          gint         *x,
                          gint         *y,
                          gint         *width,
                          gint         *height);
						  
GtkWidget *cl_special_get_ancestor(GtkWidget * widget,
				       GType widget_type);
					   
void cl_blend (GdkColormap *colormap,
            GdkColor *a, GdkColor *b, GdkColor *c, int alpha);
			
GtkWidget *cl_get_parent_window (GtkWidget *widget);

GdkColor *cl_get_parent_bgcolor (GtkWidget *widget);

gboolean cl_is_combo_box (GtkWidget * widget);

GtkWidget *cl_find_combo_box_widget (GtkWidget * widget);

void gtk_treeview_get_header_index (GtkTreeView *tv, GtkWidget *header,
                                    gint *column_index, gint *columns,
                                    gboolean *resizable);

void gtk_clist_get_header_index (GtkCList *clist, GtkWidget *button,
                                 gint *column_index, gint *columns);
void
cl_option_menu_get_props (GtkWidget      *widget,
                       GtkRequisition *indicator_size,
                       GtkBorder      *indicator_spacing);

