/* Redmond Theme Engine
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
 * modified by Andrew Johnson <ajgenius@ajgenius.us>
 */
#include <gmodule.h>
#include <gtk/gtk.h>
 
/*****************************/
/* RC Style Declaration      */
/*****************************/
extern GType redmond_type_rc_style G_GNUC_INTERNAL;
 
#define REDMOND_TYPE_RC_STYLE              redmond_type_rc_style
#define REDMOND_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), REDMOND_TYPE_RC_STYLE, RedmondRcStyle))
#define REDMOND_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), REDMOND_TYPE_RC_STYLE, RedmondRcStyleClass))
#define REDMOND_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), REDMOND_TYPE_RC_STYLE))
#define REDMOND_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), REDMOND_TYPE_RC_STYLE))
#define REDMOND_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), REDMOND_TYPE_RC_STYLE, RedmondRcStyleClass))
 
typedef struct
{
  GtkRcStyle parent_instance;
 
  GList *img_list;
} RedmondRcStyle;
 
typedef struct
{
  GtkRcStyleClass parent_class;
} RedmondRcStyleClass;
 
/*****************************/
/* Drawing Style Declaration */
/*****************************/
extern GType redmond_type_style G_GNUC_INTERNAL;
 
#define REDMOND_TYPE_STYLE              redmond_type_style
#define REDMOND_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), REDMOND_TYPE_STYLE, RedmondStyle))
#define REDMOND_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), REDMOND_TYPE_STYLE, RedmondStyleClass))
#define REDMOND_IS_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), REDMOND_TYPE_STYLE))
#define REDMOND_IS_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), REDMOND_TYPE_STYLE))
#define REDMOND_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), REDMOND_TYPE_STYLE, RedmondStyleClass))
 
typedef struct
{
  GtkStyle parent_instance;
  GdkGC *black_border_gc[5];
} RedmondStyle;
 
typedef struct
{
  GtkStyleClass parent_class;
} RedmondStyleClass;
