/*
 * HighContrast GTK+ rendering engine for Gnome-Themes.
 *
 * Copyright 2003 Sun Microsystems Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <gmodule.h>
#include <gtk/gtk.h>
 
#include "ge-support.h"

/*****************************/
/* RC Style Declaration      */
/*****************************/
GE_INTERNAL extern GType hc_type_rc_style;
GE_INTERNAL extern GtkStyleClass *hc_parent_class;
 
#define HC_TYPE_RC_STYLE              hc_type_rc_style
#define HC_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), HC_TYPE_RC_STYLE, HcRcStyle))
#define HC_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), HC_TYPE_RC_STYLE, HcRcStyleClass))
#define HC_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), HC_TYPE_RC_STYLE))
#define HC_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), HC_TYPE_RC_STYLE))
#define HC_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), HC_TYPE_RC_STYLE, HcRcStyleClass))
 
typedef enum
{
  HC_RC_FLAG_EDGE_THICKNESS      = 1 << 0,
  HC_RC_FLAG_CELL_INDICATOR_SIZE = 1 << 1
} HcRcFlags;
 
typedef struct
{
  GtkRcStyle parent_instance;
  
  HcRcFlags flags;
  
  gint edge_thickness;
  gint cell_indicator_size;
} HcRcStyle;
 
typedef struct
{
  GtkRcStyleClass parent_class;
} HcRcStyleClass;
 
/*****************************/
/* Drawing Style Declaration */
/*****************************/
GE_INTERNAL extern GType hc_type_style;
 
#define HC_TYPE_STYLE              hc_type_style
#define HC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), HC_TYPE_STYLE, HcStyle))
#define HC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), HC_TYPE_STYLE, HcStyleClass))
#define HC_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), HC_TYPE_STYLE))
#define HC_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), HC_TYPE_STYLE))
#define HC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), HC_TYPE_STYLE, HcStyleClass))
 
typedef struct
{
  GtkStyle parent_instance;

  CairoColorCube color_cube;

  gint edge_thickness;
  gint cell_indicator_size;
} HcStyle;
 
typedef struct
{
  GtkStyleClass parent_class;
} HcStyleClass;
