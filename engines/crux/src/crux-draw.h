#ifndef CRUX_DRAW_H
#define CRUX_DRAW_H

#include <gtk/gtk.h>

void crux_draw_style_class_init (GtkStyleClass *style_class) G_GNUC_INTERNAL;

extern gboolean eazel_engine_widget_in_focused_window_p (GtkWidget *widget) G_GNUC_INTERNAL;

#endif
