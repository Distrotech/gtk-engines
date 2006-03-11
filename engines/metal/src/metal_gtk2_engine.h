/* Metal Theme Engine
 * Copyright (C) 2001 Red Hat, Inc.
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
 *
 * Written by Owen Taylor <otaylor@redhat.com>
 */

#include <gmodule.h>
#include <gtk/gtk.h>
 
/*****************************/
/* RC Style Declaration      */
/*****************************/
extern GType metal_type_rc_style G_GNUC_INTERNAL;
 
#define METAL_TYPE_RC_STYLE              metal_type_rc_style
#define METAL_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), METAL_TYPE_RC_STYLE, MetalRcStyle))
#define METAL_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), METAL_TYPE_RC_STYLE, MetalRcStyleClass))
#define METAL_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), METAL_TYPE_RC_STYLE))
#define METAL_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), METAL_TYPE_RC_STYLE))
#define METAL_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), METAL_TYPE_RC_STYLE, MetalRcStyleClass))
 
typedef struct
{
  GtkRcStyle parent_instance;
 
  GList *img_list;
} MetalRcStyle;
 
typedef struct
{
  GtkRcStyleClass parent_class;
} MetalRcStyleClass;
 
/*****************************/
/* Drawing Style Declaration */
/*****************************/
extern GType metal_type_style G_GNUC_INTERNAL;
extern GtkStyleClass *parent_class G_GNUC_INTERNAL;
 
#define METAL_TYPE_STYLE              metal_type_style
#define METAL_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), METAL_TYPE_STYLE, MetalStyle))
#define METAL_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), METAL_TYPE_STYLE, MetalStyleClass))
#define METAL_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), METAL_TYPE_STYLE))
#define METAL_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), METAL_TYPE_STYLE))
#define METAL_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), METAL_TYPE_STYLE, MetalStyleClass))
 
typedef struct
{
  GtkStyle parent_instance;

  GdkColor light_gray;
  GdkColor mid_gray;
  GdkColor dark_gray;
  
  GdkGC *light_gray_gc;
  GdkGC *mid_gray_gc;
  GdkGC *dark_gray_gc;
} MetalStyle;
 
typedef struct
{
  GtkStyleClass parent_class;
} MetalStyleClass;
