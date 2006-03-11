#ifndef CRUX_PIXMAPS_H
#define CRUX_PIXMAPS_H

#include <gtk/gtk.h>

#include "crux-common.h"

extern void eazel_engine_stock_table_unref (eazel_engine_stock_table *table) G_GNUC_INTERNAL;
extern eazel_engine_stock_table *eazel_engine_stock_table_ref (eazel_engine_stock_table *table) G_GNUC_INTERNAL;
extern void eazel_engine_stock_pixmap_and_mask (eazel_engine_stock_table *table,
						eazel_engine_stock_image type,
						GdkScreen *screen,
						GdkPixmap **image,
						GdkBitmap **mask) G_GNUC_INTERNAL;
extern void eazel_engine_stock_pixmap_and_mask_scaled (eazel_engine_stock_table *table,
						       eazel_engine_stock_image type,
						       int width, int height,
						       GdkScreen *screen,
						       GdkPixmap **image,
						       GdkBitmap **mask) G_GNUC_INTERNAL;
extern void eazel_engine_stock_free_pixmaps (eazel_engine_stock_table *table,
					     eazel_engine_stock_image type,
					     GdkPixmap *image, GdkPixmap *mask) G_GNUC_INTERNAL;
extern void eazel_engine_stock_get_size (eazel_engine_stock_table *table,
					 eazel_engine_stock_image type,
					 int *width, int *height) G_GNUC_INTERNAL;

#endif
