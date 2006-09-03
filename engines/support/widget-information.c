#include <gtk/gtk.h>

#include "general-support.h"
#include "widget-information.h"
#include <math.h>

/* Widget Type Lookups/Macros
   
   Based on/modified from functions in
   Smooth-Engine.
*/ 
gboolean
ge_object_is_a (const GObject * object, const gchar * type_name)
{
  gboolean result = FALSE;
 
  if ((object))
    {
      GType tmp = g_type_from_name (type_name);

      if (tmp)
	result = g_type_check_instance_is_a ((GTypeInstance *) object, tmp);
    }
 
  return result;
}
 
gboolean
ge_is_combo_box_entry (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (GE_IS_COMBO_BOX_ENTRY (widget->parent))
	result = TRUE;
      else
	result = ge_is_combo_box_entry (widget->parent);
    }
  return result;
}
 
gboolean
ge_combo_box_is_using_list (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (GE_IS_COMBO_BOX (widget->parent)))
    {
      gboolean *tmp = NULL;
 
      gtk_widget_style_get (widget->parent, "appears-as-list", &result, NULL);
 
      if (tmp)
	result = *tmp;
    }
 
  return result;
}
 
gboolean
ge_is_combo_box (GtkWidget * widget, gboolean as_list)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (GE_IS_COMBO_BOX (widget->parent))
        {
          if (as_list)
            result = (ge_combo_box_is_using_list(widget->parent));
          else
            result = (!ge_combo_box_is_using_list(widget->parent));
        }
      else
	result = ge_is_combo_box (widget->parent, as_list);
    }
  return result;
}
 
gboolean
ge_is_combo (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (GE_IS_COMBO (widget->parent))
	result = TRUE;
      else
	result = ge_is_combo (widget->parent);
    }
  return result;
}
 
gboolean
ge_is_in_combo_box (GtkWidget * widget)
{
  return ((ge_is_combo (widget) || ge_is_combo_box (widget, TRUE) || ge_is_combo_box_entry (widget)));
}
 
gboolean
ge_is_toolbar_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent)) {
    if ((GE_IS_BONOBO_TOOLBAR (widget->parent))
	|| (GE_IS_BONOBO_DOCK_ITEM (widget->parent))
	|| (GE_IS_EGG_TOOLBAR (widget->parent))
	|| (GE_IS_TOOLBAR (widget->parent))
	|| (GE_IS_HANDLE_BOX (widget->parent)))
      result = TRUE;
    else
      result = ge_is_toolbar_item (widget->parent);
  }
  return result;
}
 
gboolean
ge_is_panel_widget_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget) && (widget->parent))
    {
      if (GE_IS_PANEL_WIDGET (widget->parent))
	result = TRUE;
      else
	result = ge_is_panel_widget_item (widget->parent);
    }
  return result;
}
 
gboolean 
ge_is_bonobo_dock_item (GtkWidget * widget)
{
  gboolean result = FALSE;
 
  if ((widget))
    {
      if (GE_IS_BONOBO_DOCK_ITEM(widget) || GE_IS_BONOBO_DOCK_ITEM (widget->parent))
	result = TRUE;
      else if (GE_IS_BOX(widget) || GE_IS_BOX(widget->parent))
        {
          GtkContainer *box = GE_IS_BOX(widget)?GTK_CONTAINER(widget):GTK_CONTAINER(widget->parent);
          GList *children = NULL, *child = NULL;
 
          children = gtk_container_get_children(box);
              
          for (child = g_list_first(children); child; child = g_list_next(child))
            {
	      if (GE_IS_BONOBO_DOCK_ITEM_GRIP(child->data))
	        {
	          result = TRUE;
	          child = NULL;
	        }
            }	            
         
          if (children)   
  	    g_list_free(children);
	}
    }
  return result;
}

static GtkWidget *
ge_find_combo_box_entry_widget (GtkWidget * widget)
{
  GtkWidget *result = NULL;

  if (widget)
    {
      if (GE_IS_COMBO_BOX_ENTRY (widget))
	result = widget;
      else
	result = ge_find_combo_box_entry_widget (widget->parent);
    }

  return result;
}

static GtkWidget *
ge_find_combo_box_widget (GtkWidget * widget, gboolean as_list)
{
  GtkWidget *result = NULL;
 
  if (widget)
    {
      if (GE_IS_COMBO_BOX (widget))
        {
          if (as_list)
            result = (ge_combo_box_is_using_list(widget))?widget:NULL;
          else
            result = (!ge_combo_box_is_using_list(widget))?widget:NULL;
        }
      else
	result = ge_find_combo_box_widget (widget->parent, as_list);
    }
  return result;
}
 
static GtkWidget *
ge_find_combo_widget (GtkWidget * widget)
{
  GtkWidget *result = NULL;
 
  if (widget)
    {
      if (GE_IS_COMBO (widget))
	result = widget;
      else
	result = ge_find_combo_widget(widget->parent);
    }
  return result;
}

GtkWidget*
ge_find_combo_box_widget_parent (GtkWidget * widget)
{
   GtkWidget *result = NULL;
   
   if (!result)
     result = ge_find_combo_widget(widget);
  
   if (!result)
     result = ge_find_combo_box_widget(widget, TRUE);

   if (!result)
     result = ge_find_combo_box_entry_widget(widget);

  return result;
}

gboolean
ge_cell_renderer_toggle_get_inconsistent (GtkWidget * widget)
{
  gboolean result = FALSE;
  
  g_object_get (widget, "inconsistent", &result, NULL);
  
  return result;
}

gboolean
ge_toggle_get_inconsistent (GtkWidget * widget, const gchar *detail, GtkShadowType shadow_type)
{
	return (GE_IS_TOGGLE_BUTTON(widget) && gtk_toggle_button_get_inconsistent(TOGGLE_BUTTON(widget)))
				| (GE_IS_CELL_RENDERER_TOGGLE(widget) && ge_cell_renderer_toggle_get_inconsistent (widget))
				| (CHECK_DETAIL(detail, "cellcheck") && (shadow_type == GTK_SHADOW_ETCHED_IN))
				| (CHECK_DETAIL(detail, "cellradio") && (shadow_type == GTK_SHADOW_ETCHED_IN));
}

/***********************************************
 * option_menu_get_props -
 *  
 *   Find Option Menu Size and Spacing
 *
 *   Taken from Smooth
 ***********************************************/ 
void
ge_option_menu_get_props (GtkWidget * widget,
		       GtkRequisition * indicator_size,
		       GtkBorder * indicator_spacing)
{
  GtkRequisition default_size = { 9, 5 };
  GtkBorder default_spacing = { 7, 5, 2, 2 };
  GtkRequisition *tmp_size = NULL;
  GtkBorder *tmp_spacing = NULL;
 
  if ((widget) && GE_IS_OPTION_MENU(widget))
    gtk_widget_style_get (widget,
			  "indicator_size", &tmp_size,
			  "indicator_spacing", &tmp_spacing, NULL);
 
  if (tmp_size)
    {
      *indicator_size = *tmp_size;
      g_free (tmp_size);
    }
  else
    *indicator_size = default_size;
 
  if (tmp_spacing)
    {
      *indicator_spacing = *tmp_spacing;
      g_free (tmp_spacing);
    }
  else
    *indicator_spacing = default_spacing;
}

void
ge_button_get_default_border (GtkWidget *widget, 
                              GtkBorder *border)
{
	GtkBorder *tmp_border = NULL;
	
	if (widget && GE_IS_BUTTON (widget))
		gtk_widget_style_get (widget, "default-border", &tmp_border, NULL);

	if (tmp_border)
	{
		*border = *tmp_border;
		g_free (tmp_border);
	}
	else
	{
		*border = (GtkBorder){1, 1, 1, 1};
	}
}


gboolean
ge_widget_is_ltr (GtkWidget *widget)
{
	GtkTextDirection dir = GTK_TEXT_DIR_NONE;
	
	if (GE_IS_WIDGET (widget))
		dir = gtk_widget_get_direction (widget);

	if (dir == GTK_TEXT_DIR_NONE)
		dir = gtk_widget_get_default_direction ();

	if (dir == GTK_TEXT_DIR_RTL)
		return FALSE;
	else
		return TRUE;
}
