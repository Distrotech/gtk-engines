/* Metal theme engine
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
#define CHECK_DETAIL(detail, value) ((detail) && (!strcmp(value, detail)))
  
/* Object Type Lookups/Macros
   
   Based on/modified from functions in
   Smooth-Engine.
*/ 
#define IS_WIDGET(object) ((object)  && metal_object_is_a (object, "GtkWidget"))
#define IS_CONTAINER(object) ((object)  && metal_object_is_a (object, "GtkContainer"))
#define IS_BIN(object) ((object)  && metal_object_is_a (object, "GtkBin"))
 
#define IS_HANDLE_BOX(object) ((object) && metal_object_is_a (object, "GtkHandleBox"))
#define IS_HANDLE_BOX_ITEM(object) ((object) && IS_HANDLE_BOX(object->parent))
#define IS_BONOBO_DOCK_ITEM(object) ((object) && metal_object_is_a (object, "BonoboDockItem"))
#define IS_BONOBO_DOCK_ITEM_GRIP(object) ((object) && metal_object_is_a (object, "BonoboDockItemGrip"))
#define IS_BONOBO_TOOLBAR(object) ((object) && metal_object_is_a (object, "BonoboUIToolbar"))
#define IS_EGG_TOOLBAR(object) ((object) && metal_object_is_a (object, "Toolbar"))
#define IS_TOOLBAR(object) ((object) && metal_object_is_a (object, "GtkToolbar"))
#define IS_PANEL_WIDGET(object) ((object) && metal_object_is_a (object, "PanelWidget"))
 
#define IS_COMBO_BOX_ENTRY(object) ((widget) && metal_object_is_a (widget, "GtkComboBoxEntry"))
#define IS_COMBO_BOX(object) ((widget) && metal_object_is_a (widget, "GtkComboBox"))
#define IS_COMBO(object) ((widget) && metal_object_is_a (widget, "GtkCombo"))
#define IS_OPTION_MENU(object) ((widget) && metal_object_is_a (widget, "GtkOptionMenu"))
 
#define IS_TOGGLE_BUTTON(object) ((object) && metal_object_is_a (object, "GtkToggleButton"))
#define IS_SPIN_BUTTON(object) ((object) && metal_object_is_a (object, "GtkSpinButton"))
 
#define IS_STATUS_BAR(object) ((object) && metal_object_is_a (object, "GtkStatusBar"))
#define IS_PROGRESS_BAR(object) ((object) && metal_object_is_a (object, "GtkProgressBar"))
 
#define IS_MENU_SHELL(object) ((object) && metal_object_is_a (object, "GtkMenuShell"))
#define IS_MENU(object) ((object) && metal_object_is_a (object, "GtkMenu"))
#define IS_MENU_BAR(object) ((object) && metal_object_is_a (object, "GtkMenuBar"))
#define IS_MENU_ITEM(object) ((object) && metal_object_is_a (object, "GtkMenuItem"))
 
#define IS_SCROLLBAR(object) ((object) && metal_object_is_a (object, "GtkScrollbar"))
#define IS_VSCROLLBAR(object) ((object) && metal_object_is_a (object, "GtkVScrollbar"))
#define IS_HSCROLLBAR(object) ((object) && metal_object_is_a (object, "GtkHScrollbar"))
 
#define IS_SCALE(object) ((object) && metal_object_is_a (object, "GtkScale"))
#define IS_VSCALE(object) ((object) && metal_object_is_a (object, "GtkVScale"))
#define IS_HSCALE(object) ((object) && metal_object_is_a (object, "GtkHScale"))
  
#define IS_PANED(object) ((object) && metal_object_is_a (object, "GtkPaned"))
#define IS_VPANED(object) ((object) && metal_object_is_a (object, "GtkVPaned"))
#define IS_HPANED(object) ((object) && metal_object_is_a (object, "GtkHPaned"))
 
#define IS_BOX(object) ((object) && metal_object_is_a (object, "GtkBox"))
#define IS_VBOX(object) ((object) && metal_object_is_a (object, "GtkVBox"))
#define IS_HBOX(object) ((object) && metal_object_is_a (object, "GtkHBox"))
 
#define TOGGLE_BUTTON(object) (IS_TOGGLE_BUTTON(object)?(GtkToggleButton *)object:NULL)
 
gboolean metal_object_is_a (GtkWidget * widget, gchar * type_name) G_GNUC_INTERNAL;
 
gboolean metal_is_combo_box_entry (GtkWidget * widget) G_GNUC_INTERNAL;
gboolean metal_is_combo_box (GtkWidget * widget, gboolean as_list) G_GNUC_INTERNAL;
gboolean metal_is_combo (GtkWidget * widget) G_GNUC_INTERNAL;
gboolean metal_is_in_combo_box (GtkWidget * widget) G_GNUC_INTERNAL;
  
/* Widget Sizing */ 
gboolean metal_sanitize_parameters(GtkStyle * style, 
                             GdkWindow * window, 
                             gint * width, 
                             gint * height) G_GNUC_INTERNAL;

void
do_metal_draw_arrow (GdkWindow * window,
               GtkWidget  * widget,
               GdkGC * gc,
               GtkArrowType arrow_type,
               gint x, 
               gint y, 
               gint width, 
               gint height) G_GNUC_INTERNAL;

void
do_metal_draw_scrollbar_trough (GtkStyle     *style,
			GdkWindow    *window,
			GtkStateType  state_type,
			GtkShadowType shadow_type,
			GdkRectangle *area,
			GtkWidget    *widget,
			const char   *detail,
			gint          x,
			gint          y,
			gint          width,
			gint          height) G_GNUC_INTERNAL;

void
do_metal_draw_scale_trough (GtkStyle * style,
		    GdkWindow * window,
		    GtkStateType state_type,
		    GtkShadowType shadow_type,
		    GdkRectangle * area,
		    GtkWidget * widget,
		    const gchar * detail,
		    gint x,
		    gint y,
		    gint width,
		    gint height) G_GNUC_INTERNAL;

void
do_metal_draw_menu (GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    const gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height) G_GNUC_INTERNAL;

void
do_metal_draw_menu_item (GtkStyle * style,
		 GdkWindow * window,
		 GtkStateType state_type,
		 GtkShadowType shadow_type,
		 GdkRectangle * area,
		 GtkWidget * widget,
		 const gchar * detail,
		 gint x,
		 gint y,
		 gint width,
		 gint height) G_GNUC_INTERNAL;

void
do_metal_draw_notebook (GtkStyle * style,
		GdkWindow * window,
		GtkStateType state_type,
		GtkShadowType shadow_type,
		GdkRectangle * area,
		GtkWidget * widget,
		const gchar * detail,
		gint x,
		gint y,
		gint width,
		gint height) G_GNUC_INTERNAL;

void
do_metal_draw_notebook_tab (GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   const gchar * detail,
	   gint x,
	   gint y,
	   gint width,
	   gint height,
	   gint orientation) G_GNUC_INTERNAL;

void
do_metal_draw_button (GtkStyle * style,
	      GdkWindow * window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle * area,
	      GtkWidget * widget,
	      const gchar * detail,
	      gint x,
	      gint y,
	      gint width,
	      gint height) G_GNUC_INTERNAL;

void
metal_adjust_notebook_tab_size (GtkPositionType tab_pos,
                          gint           *width,
                          gint           *height) G_GNUC_INTERNAL;

void
metal_reverse_engineer_stepper_box (GtkWidget    *range,
                              GtkArrowType  arrow_type,
                              gint         *x,
                              gint         *y,
                              gint         *width,
                              gint         *height) G_GNUC_INTERNAL;

