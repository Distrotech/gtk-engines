#include <gtk/gtk.h>

extern GtkStyleClass motif_default_class;

/* internal functions */
static void         draw_hline(GtkStyle * style,
			       GdkWindow * window,
			       GtkStateType state_type,
			       GdkRectangle * area,
			       GtkWidget * widget,
			       gchar * detail,
			       gint x1,
			       gint x2,
			       gint y);
static void         draw_vline(GtkStyle * style,
			       GdkWindow * window,
			       GtkStateType state_type,
			       GdkRectangle * area,
			       GtkWidget * widget,
			       gchar * detail,
			       gint y1,
			       gint y2,
			       gint x);
static void         draw_shadow(GtkStyle * style,
				GdkWindow * window,
				GtkStateType state_type,
				GtkShadowType shadow_type,
				GdkRectangle * area,
				GtkWidget * widget,
				gchar * detail,
				gint x,
				gint y,
				gint width,
				gint height);

static void         draw_polygon(GtkStyle * style,
				 GdkWindow * window,
				 GtkStateType state_type,
				 GtkShadowType shadow_type,
				 GdkRectangle * area,
				 GtkWidget * widget,
				 gchar * detail,
				 GdkPoint * point,
				 gint npoints,
				 gint fill);
static void         draw_arrow(GtkStyle * style,
			       GdkWindow * window,
			       GtkStateType state_type,
			       GtkShadowType shadow_type,
			       GdkRectangle * area,
			       GtkWidget * widget,
			       gchar * detail,
			       GtkArrowType arrow_type,
			       gint fill,
			       gint x,
			       gint y,
			       gint width,
			       gint height);
static void         draw_diamond(GtkStyle * style,
				 GdkWindow * window,
				 GtkStateType state_type,
				 GtkShadowType shadow_type,
				 GdkRectangle * area,
				 GtkWidget * widget,
				 gchar * detail,
				 gint x,
				 gint y,
				 gint width,
				 gint height);
static void         draw_oval(GtkStyle * style,
			      GdkWindow * window,
			      GtkStateType state_type,
			      GtkShadowType shadow_type,
			      GdkRectangle * area,
			      GtkWidget * widget,
			      gchar * detail,
			      gint x,
			      gint y,
			      gint width,
			      gint height);
static void         draw_string(GtkStyle * style,
				GdkWindow * window,
				GtkStateType state_type,
				GdkRectangle * area,
				GtkWidget * widget,
				gchar * detail,
				gint x,
				gint y,
				const gchar * string);
static void         draw_box(GtkStyle * style,
			     GdkWindow * window,
			     GtkStateType state_type,
			     GtkShadowType shadow_type,
			     GdkRectangle * area,
			     GtkWidget * widget,
			     gchar * detail,
			     gint x,
			     gint y,
			     gint width,
			     gint height);
static void         draw_flat_box(GtkStyle * style,
				  GdkWindow * window,
				  GtkStateType state_type,
				  GtkShadowType shadow_type,
				  GdkRectangle * area,
				  GtkWidget * widget,
				  gchar * detail,
				  gint x,
				  gint y,
				  gint width,
				  gint height);
static void         draw_check(GtkStyle * style,
			       GdkWindow * window,
			       GtkStateType state_type,
			       GtkShadowType shadow_type,
			       GdkRectangle * area,
			       GtkWidget * widget,
			       gchar * detail,
			       gint x,
			       gint y,
			       gint width,
			       gint height);
static void         draw_option(GtkStyle * style,
				GdkWindow * window,
				GtkStateType state_type,
				GtkShadowType shadow_type,
				GdkRectangle * area,
				GtkWidget * widget,
				gchar * detail,
				gint x,
				gint y,
				gint width,
				gint height);
static void         draw_cross(GtkStyle * style,
			       GdkWindow * window,
			       GtkStateType state_type,
			       GtkShadowType shadow_type,
			       GdkRectangle * area,
			       GtkWidget * widget,
			       gchar * detail,
			       gint x,
			       gint y,
			       gint width,
			       gint height);
static void         draw_ramp(GtkStyle * style,
			      GdkWindow * window,
			      GtkStateType state_type,
			      GtkShadowType shadow_type,
			      GdkRectangle * area,
			      GtkWidget * widget,
			      gchar * detail,
			      GtkArrowType arrow_type,
			      gint x,
			      gint y,
			      gint width,
			      gint height);
static void         draw_tab(GtkStyle * style,
			     GdkWindow * window,
			     GtkStateType state_type,
			     GtkShadowType shadow_type,
			     GdkRectangle * area,
			     GtkWidget * widget,
			     gchar * detail,
			     gint x,
			     gint y,
			     gint width,
			     gint height);
static void         draw_shadow_gap(GtkStyle * style,
				    GdkWindow * window,
				    GtkStateType state_type,
				    GtkShadowType shadow_type,
				    GdkRectangle * area,
				    GtkWidget * widget,
				    gchar * detail,
				    gint x,
				    gint y,
				    gint width,
				    gint height,
				    gint gap_side,
				    gint gap_x,
				    gint gap_width);
static void         draw_box_gap(GtkStyle * style,
				 GdkWindow * window,
				 GtkStateType state_type,
				 GtkShadowType shadow_type,
				 GdkRectangle * area,
				 GtkWidget * widget,
				 gchar * detail,
				 gint x,
				 gint y,
				 gint width,
				 gint height,
				 gint gap_side,
				 gint gap_x,
				 gint gap_width);
static void         draw_extension(GtkStyle * style,
				   GdkWindow * window,
				   GtkStateType state_type,
				   GtkShadowType shadow_type,
				   GdkRectangle * area,
				   GtkWidget * widget,
				   gchar * detail,
				   gint x,
				   gint y,
				   gint width,
				   gint height,
				   gint gap_side);
static void         draw_focus(GtkStyle * style,
			       GdkWindow * window,
			       GdkRectangle * area,
			       GtkWidget * widget,
			       gchar * detail,
			       gint x,
			       gint y,
			       gint width,
			       gint height);
static void         draw_slider(GtkStyle * style,
				GdkWindow * window,
				GtkStateType state_type,
				GtkShadowType shadow_type,
				GdkRectangle * area,
				GtkWidget * widget,
				gchar * detail,
				gint x,
				gint y,
				gint width,
				gint height,
				GtkOrientation orientation);
static void         draw_entry(GtkStyle * style,
			       GdkWindow * window,
			       GtkStateType state_type,
			       GdkRectangle * area,
			       GtkWidget * widget,
			       gchar * detail,
			       gint x,
			       gint y,
			       gint width,
			       gint height);
static void         draw_handle(GtkStyle * style,
				GdkWindow * window,
				GtkStateType state_type,
				GtkShadowType shadow_type,
				GdkRectangle * area,
				GtkWidget * widget,
				gchar * detail,
				gint x,
				gint y,
				gint width,
				gint height,
				GtkOrientation orientation);

/* internal data structs */

GtkStyleClass       motif_default_class =
{
  2,
  2,
  draw_hline,
  draw_vline,
  draw_shadow,
  draw_polygon,
  draw_arrow,
  draw_diamond,
  draw_oval,
  draw_string,
  draw_box,
  draw_flat_box,
  draw_check,
  draw_option,
  draw_cross,
  draw_ramp,
  draw_tab,
  draw_shadow_gap,
  draw_box_gap,
  draw_extension,
  draw_focus,
  draw_slider,
  draw_entry,
  draw_handle
};

static void
draw_hline(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   gchar * detail,
	   gint x1,
	   gint x2,
	   gint y)
{
  gint                thickness_light;
  gint                thickness_dark;
  gint                i;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  thickness_light = style->klass->ythickness / 2;
  thickness_dark = style->klass->ythickness - thickness_light;

  if (area)
    {
      gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
      gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
    }
  for (i = 0; i < thickness_dark; i++)
    {
      gdk_draw_line(window, style->light_gc[state_type], x2 - i - 1, y + i, x2, y + i);
      gdk_draw_line(window, style->dark_gc[state_type], x1, y + i, x2 - i - 1, y + i);
    }

  y += thickness_dark;
  for (i = 0; i < thickness_light; i++)
    {
      gdk_draw_line(window, style->dark_gc[state_type], x1, y + i, x1 + thickness_light - i - 1, y + i);
      gdk_draw_line(window, style->light_gc[state_type], x1 + thickness_light - i - 1, y + i, x2, y + i);
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
    }
}

static void
draw_vline(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   gchar * detail,
	   gint y1,
	   gint y2,
	   gint x)
{
  gint                thickness_light;
  gint                thickness_dark;
  gint                i;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  thickness_light = style->klass->xthickness / 2;
  thickness_dark = style->klass->xthickness - thickness_light;

  if (area)
    {
      gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
      gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
    }
  for (i = 0; i < thickness_dark; i++)
    {
      gdk_draw_line(window, style->light_gc[state_type], x + i, y2 - i - 1, x + i, y2);
      gdk_draw_line(window, style->dark_gc[state_type], x + i, y1, x + i, y2 - i - 1);
    }

  x += thickness_dark;
  for (i = 0; i < thickness_light; i++)
    {
      gdk_draw_line(window, style->dark_gc[state_type], x + i, y1, x + i, y1 + thickness_light - i);
      gdk_draw_line(window, style->light_gc[state_type], x + i, y1 + thickness_light - i, x + i, y2);
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
    }
}

static void
draw_shadow(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height)
{
  GdkGC              *gc1 = NULL;
  GdkGC              *gc2 = NULL;
  gint                thickness_light;
  gint                thickness_dark;
  gint                i;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      return;
    case GTK_SHADOW_IN:
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      break;
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle(gc1, area);
      gdk_gc_set_clip_rectangle(gc2, area);
      if ((shadow_type == GTK_SHADOW_IN) ||
	  (shadow_type == GTK_SHADOW_OUT))
	{
	  gdk_gc_set_clip_rectangle(style->black_gc, area);
	  gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);
	}
    }
  switch (shadow_type)
    {
    case GTK_SHADOW_NONE:
      break;

    case GTK_SHADOW_IN:
      gdk_draw_line(window, gc1,
		    x, y + height - 1, x + width - 1, y + height - 1);
      gdk_draw_line(window, gc1,
		    x + width - 1, y, x + width - 1, y + height - 1);

      gdk_draw_line(window, gc1,
		    x + 1, y + height - 2, x + width - 2, y + height - 2);
      gdk_draw_line(window, gc1,
		    x + width - 2, y + 1, x + width - 2, y + height - 2);

      gdk_draw_line(window, gc2,
		    x + 1, y + 1, x + width - 2, y + 1);
      gdk_draw_line(window, gc2,
		    x + 1, y + 1, x + 1, y + height - 2);

      gdk_draw_line(window, gc2,
		    x, y, x + width - 1, y);
      gdk_draw_line(window, gc2,
		    x, y, x, y + height - 1);
      break;

    case GTK_SHADOW_OUT:
      gdk_draw_line(window, gc1,
		    x + 1, y + height - 2, x + width - 2, y + height - 2);
      gdk_draw_line(window, gc1,
		    x + width - 2, y + 1, x + width - 2, y + height - 2);

      gdk_draw_line(window, gc2,
		    x, y, x + width - 1, y);
      gdk_draw_line(window, gc2,
		    x, y, x, y + height - 1);

      gdk_draw_line(window, gc2,
		    x + 1, y + 1, x + width - 2, y + 1);
      gdk_draw_line(window, gc2,
		    x + 1, y + 1, x + 1, y + height - 2);

      gdk_draw_line(window, gc1,
		    x, y + height - 1, x + width - 1, y + height - 1);
      gdk_draw_line(window, gc1,
		    x + width - 1, y, x + width - 1, y + height - 1);
      break;

    case GTK_SHADOW_ETCHED_IN:
    case GTK_SHADOW_ETCHED_OUT:
      thickness_light = 1;
      thickness_dark = 1;

      for (i = 0; i < thickness_dark; i++)
	{
	  gdk_draw_line(window, gc1,
			x + i,
			y + height - i - 1,
			x + width - i - 1,
			y + height - i - 1);
	  gdk_draw_line(window, gc1,
			x + width - i - 1,
			y + i,
			x + width - i - 1,
			y + height - i - 1);

	  gdk_draw_line(window, gc2,
			x + i,
			y + i,
			x + width - i - 2,
			y + i);
	  gdk_draw_line(window, gc2,
			x + i,
			y + i,
			x + i,
			y + height - i - 2);
	}

      for (i = 0; i < thickness_light; i++)
	{
	  gdk_draw_line(window, gc1,
			x + thickness_dark + i,
			y + thickness_dark + i,
			x + width - thickness_dark - i - 1,
			y + thickness_dark + i);
	  gdk_draw_line(window, gc1,
			x + thickness_dark + i,
			y + thickness_dark + i,
			x + thickness_dark + i,
			y + height - thickness_dark - i - 1);

	  gdk_draw_line(window, gc2,
			x + thickness_dark + i,
			y + height - thickness_light - i - 1,
			x + width - thickness_light - 1,
			y + height - thickness_light - i - 1);
	  gdk_draw_line(window, gc2,
			x + width - thickness_light - i - 1,
			y + thickness_dark + i,
			x + width - thickness_light - i - 1,
			y + height - thickness_light - 1);
	}
      break;
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle(gc1, NULL);
      gdk_gc_set_clip_rectangle(gc2, NULL);
      if ((shadow_type == GTK_SHADOW_IN) ||
	  (shadow_type == GTK_SHADOW_OUT))
	{
	  gdk_gc_set_clip_rectangle(style->black_gc, NULL);
	  gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);
	}
    }
}

static void
draw_polygon(GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     gchar * detail,
	     GdkPoint * points,
	     gint npoints,
	     gint fill)
{
#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif /* M_PI */
#ifndef M_PI_4
#define M_PI_4  0.78539816339744830962
#endif /* M_PI_4 */

  static const gdouble pi_over_4 = M_PI_4;
  static const gdouble pi_3_over_4 = M_PI_4 * 3;

  GdkGC              *gc1;
  GdkGC              *gc2;
  GdkGC              *gc3;
  GdkGC              *gc4;
  gdouble             angle;
  gint                xadjust;
  gint                yadjust;
  gint                i;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);
  g_return_if_fail(points != NULL);

  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->dark_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->dark_gc[state_type];
      break;
    default:
      return;
    }

  if (area)
    {
      gdk_gc_set_clip_rectangle(gc1, area);
      gdk_gc_set_clip_rectangle(gc2, area);
      gdk_gc_set_clip_rectangle(gc3, area);
      gdk_gc_set_clip_rectangle(gc4, area);
    }

  if (fill)
    gdk_draw_polygon(window, style->bg_gc[state_type], TRUE, points, npoints);

  npoints--;

  for (i = 0; i < npoints; i++)
    {
      if ((points[i].x == points[i + 1].x) &&
	  (points[i].y == points[i + 1].y))
	{
	  angle = 0;
	}
      else
	{
	  angle = atan2(points[i + 1].y - points[i].y,
			points[i + 1].x - points[i].x);
	}

      if ((angle > -pi_3_over_4) && (angle < pi_over_4))
	{
	  if (angle > -pi_over_4)
	    {
	      xadjust = 0;
	      yadjust = 1;
	    }
	  else
	    {
	      xadjust = 1;
	      yadjust = 0;
	    }

	  gdk_draw_line(window, gc1,
			points[i].x - xadjust, points[i].y - yadjust,
			points[i + 1].x - xadjust, points[i + 1].y - yadjust);
	  gdk_draw_line(window, gc3,
			points[i].x, points[i].y,
			points[i + 1].x, points[i + 1].y);
	}
      else
	{
	  if ((angle < -pi_3_over_4) || (angle > pi_3_over_4))
	    {
	      xadjust = 0;
	      yadjust = 1;
	    }
	  else
	    {
	      xadjust = 1;
	      yadjust = 0;
	    }

	  gdk_draw_line(window, gc4,
			points[i].x + xadjust, points[i].y + yadjust,
			points[i + 1].x + xadjust, points[i + 1].y + yadjust);
	  gdk_draw_line(window, gc2,
			points[i].x, points[i].y,
			points[i + 1].x, points[i + 1].y);
	}
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle(gc1, NULL);
      gdk_gc_set_clip_rectangle(gc2, NULL);
      gdk_gc_set_clip_rectangle(gc3, NULL);
      gdk_gc_set_clip_rectangle(gc4, NULL);
    }
}

static void
draw_arrow(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   gchar * detail,
	   GtkArrowType arrow_type,
	   gint fill,
	   gint x,
	   gint y,
	   gint width,
	   gint height)
{
  GdkGC              *gc1;
  GdkGC              *gc2;
  GdkGC              *gc3;
  GdkGC              *gc4;
  gint                half_width;
  gint                half_height;
  GdkPoint            points[3];

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = style->light_gc[state_type];
      gc4 = style->dark_gc[state_type];
      break;
    case GTK_SHADOW_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = style->dark_gc[state_type];
      gc4 = style->light_gc[state_type];
      break;
    case GTK_SHADOW_ETCHED_IN:
      gc1 = style->light_gc[state_type];
      gc2 = style->dark_gc[state_type];
      gc3 = NULL;
      gc4 = NULL;
      break;
    case GTK_SHADOW_ETCHED_OUT:
      gc1 = style->dark_gc[state_type];
      gc2 = style->light_gc[state_type];
      gc3 = NULL;
      gc4 = NULL;
      break;
    default:
      return;
    }

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  half_width = width / 2;
  half_height = height / 2;

  if (area)
    {
      gdk_gc_set_clip_rectangle(gc1, area);
      gdk_gc_set_clip_rectangle(gc2, area);
      if ((gc3) && (gc4))
	{
	  gdk_gc_set_clip_rectangle(gc3, area);
	  gdk_gc_set_clip_rectangle(gc4, area);
	}
    }

  switch (arrow_type)
    {
    case GTK_ARROW_UP:
      if (fill)
	{
	  points[0].x = x + half_width;
	  points[0].y = y;
	  points[1].x = x;
	  points[1].y = y + height - 1;
	  points[2].x = x + width - 1;
	  points[2].y = y + height - 1;

	  gdk_draw_polygon(window, style->bg_gc[state_type], TRUE, points, 3);
	}

      switch (shadow_type)
	{
	case GTK_SHADOW_IN:
	case GTK_SHADOW_OUT:

	  gdk_draw_line(window, gc1,
			x + 1, y + height - 2,
			x + width - 2, y + height - 2);
	  gdk_draw_line(window, gc3,
			x + 0, y + height - 1,
			x + width - 1, y + height - 1);

	  gdk_draw_line(window, gc1,
			x + width - 2, y + height - 1,
			x + half_width, y + 1);
	  gdk_draw_line(window, gc3,
			x + width - 1, y + height - 1,
			x + half_width, y);

	  gdk_draw_line(window, gc4,
			x + half_width, y + 1,
			x + 1, y + height - 1);
	  gdk_draw_line(window, gc2,
			x + half_width, y,
			x, y + height - 1);
	  break;

	case GTK_SHADOW_ETCHED_IN:
	case GTK_SHADOW_ETCHED_OUT:
	  gdk_draw_line(window, gc1,
			x + half_width, y + 1,
			x + 1, y + height - 1);
	  gdk_draw_line(window, gc1,
			x + 1, y + height - 1,
			x + width - 1, y + height - 1);
	  gdk_draw_line(window, gc1,
			x + width - 1, y + height - 1,
			x + half_width + 1, y + 1);

	  points[0].x = x + half_width;
	  points[0].y = y;
	  points[1].x = x;
	  points[1].y = y + height - 2;
	  points[2].x = x + width - 2;
	  points[2].y = y + height - 2;

	  gdk_draw_polygon(window, gc2, FALSE, points, 3);
	  break;

	default:
	  break;
	}
      break;

    case GTK_ARROW_DOWN:
      if (fill)
	{
	  points[0].x = x + width - 1;
	  points[0].y = y;
	  points[1].x = x;
	  points[1].y = y;
	  points[2].x = x + half_width;
	  points[2].y = y + height - 1;

	  gdk_draw_polygon(window, style->bg_gc[state_type], TRUE, points, 3);
	}
      switch (shadow_type)
	{
	case GTK_SHADOW_IN:
	case GTK_SHADOW_OUT:
	  gdk_draw_line(window, gc4,
			x + width - 2,
			y + 1, x + 1, y + 1);
	  gdk_draw_line(window, gc2,
			x + width - 1, y,
			x, y);

	  gdk_draw_line(window, gc4,
			x + 1, y,
			x + half_width, y + height - 2);
	  gdk_draw_line(window, gc2,
			x, y,
			x + half_width, y + height - 1);

	  gdk_draw_line(window, gc1,
			x + half_width, y + height - 2,
			x + width - 2, y);
	  gdk_draw_line(window, gc3,
			x + half_width, y + height - 1,
			x + width - 1, y);
	  break;

	case GTK_SHADOW_ETCHED_IN:
	case GTK_SHADOW_ETCHED_OUT:
	  gdk_draw_line(window, gc1,
			x + width - 1, y + 1,
			x + 1, y + 1);
	  gdk_draw_line(window, gc1,
			x + 1, y + 1,
			x + half_width + 1, y + height - 1);
	  gdk_draw_line(window, gc1,
			x + half_width + 1, y + height - 2,
			x + width - 1, y);

	  points[0].x = x + width - 2;
	  points[0].y = y;
	  points[1].x = x;
	  points[1].y = y;
	  points[2].x = x + half_width;
	  points[2].y = y + height - 2;

	  gdk_draw_polygon(window, gc2, FALSE, points, 3);
	  break;

	default:
	  break;
	}
      break;
    case GTK_ARROW_LEFT:
      if (fill)
	{
	  points[0].x = x;
	  points[0].y = y + half_height;
	  points[1].x = x + width - 1;
	  points[1].y = y + height - 1;
	  points[2].x = x + width - 1;
	  points[2].y = y;

	  gdk_draw_polygon(window, style->bg_gc[state_type], TRUE, points, 3);
	}

      switch (shadow_type)
	{
	case GTK_SHADOW_IN:
	case GTK_SHADOW_OUT:
	  gdk_draw_line(window, gc1,
			x + 1, y + half_height,
			x + width - 1, y + height - 1);
	  gdk_draw_line(window, gc3,
			x, y + half_height,
			x + width - 1, y + height - 1);

	  gdk_draw_line(window, gc1,
			x + width - 2, y + height - 1,
			x + width - 2, y + 1);
	  gdk_draw_line(window, gc3,
			x + width - 1, y + height - 1,
			x + width - 1, y);

	  gdk_draw_line(window, gc4,
			x + width - 1, y + 1,
			x + 1, y + half_height);
	  gdk_draw_line(window, gc2,
			x + width - 1, y,
			x, y + half_height);
	  break;

	case GTK_SHADOW_ETCHED_IN:
	case GTK_SHADOW_ETCHED_OUT:
	  gdk_draw_line(window, gc1,
			x + width - 1, y + 1,
			x + 1, y + half_height);
	  gdk_draw_line(window, gc1,
			x + 1, y + half_height + 1,
			x + width - 1, y + height - 1);
	  gdk_draw_line(window, gc1,
			x + width - 1, y + height - 1,
			x + width - 1, y + 1);

	  points[0].x = x + width - 2;
	  points[0].y = y;
	  points[1].x = x;
	  points[1].y = y + half_height;
	  points[2].x = x + width - 2;
	  points[2].y = y + height - 2;

	  gdk_draw_polygon(window, gc2, FALSE, points, 3);
	  break;

	default:
	  break;
	}
      break;
    case GTK_ARROW_RIGHT:
      if (fill)
	{
	  points[0].x = x + width - 1;
	  points[0].y = y + half_height;
	  points[1].x = x;
	  points[1].y = y;
	  points[2].x = x;
	  points[2].y = y + height - 1;

	  gdk_draw_polygon(window, style->bg_gc[state_type], TRUE, points, 3);
	}

      switch (shadow_type)
	{
	case GTK_SHADOW_IN:
	case GTK_SHADOW_OUT:
	  gdk_draw_line(window, gc4,
			x + width - 1, y + half_height,
			x + 1, y + 1);
	  gdk_draw_line(window, gc2,
			x + width - 1, y + half_height,
			x, y);
	  gdk_draw_line(window, gc4,
			x + 1, y + 1,
			x + 1, y + height - 2);
	  gdk_draw_line(window, gc2,
			x, y,
			x, y + height - 1);

	  gdk_draw_line(window, gc1,
			x + 1, y + height - 2,
			x + width - 1, y + half_height);
	  gdk_draw_line(window, gc3,
			x, y + height - 1,
			x + width - 1, y + half_height);
	  break;

	case GTK_SHADOW_ETCHED_IN:
	case GTK_SHADOW_ETCHED_OUT:
	  gdk_draw_line(window, gc1,
			x + width - 1, y + half_height + 1,
			x + 1, y + 1);
	  gdk_draw_line(window, gc1,
			x + 1, y + 1,
			x + 1, y + height - 1);
	  gdk_draw_line(window, gc1,
			x + 1, y + height - 1,
			x + width - 1, y + half_height + 1);

	  points[0].x = x + width - 2;
	  points[0].y = y + half_height;
	  points[1].x = x;
	  points[1].y = y;
	  points[2].x = x;
	  points[2].y = y + height - 1;

	  gdk_draw_polygon(window, gc2, FALSE, points, 3);
	  break;

	default:
	  break;
	}
      break;
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle(gc1, NULL);
      gdk_gc_set_clip_rectangle(gc2, NULL);
      if (gc3)
	{
	  gdk_gc_set_clip_rectangle(gc3, NULL);
	  gdk_gc_set_clip_rectangle(gc4, NULL);
	}
    }
}

static void
draw_diamond(GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     gchar * detail,
	     gint x,
	     gint y,
	     gint width,
	     gint height)
{
  gint                half_width;
  gint                half_height;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  half_width = width / 2;
  half_height = height / 2;

  if (area)
    {
      gdk_gc_set_clip_rectangle(style->light_gc[state_type], area);
      gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);
      gdk_gc_set_clip_rectangle(style->dark_gc[state_type], area);
      gdk_gc_set_clip_rectangle(style->black_gc, area);
    }
  switch (shadow_type)
    {
    case GTK_SHADOW_IN:
      gdk_draw_line(window, style->light_gc[state_type],
		    x + 2, y + half_height,
		    x + half_width, y + height - 2);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + half_width, y + height - 2,
		    x + width - 2, y + half_height);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + 1, y + half_height,
		    x + half_width, y + height - 1);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + half_width, y + height - 1,
		    x + width - 1, y + half_height);
      gdk_draw_line(window, style->light_gc[state_type],
		    x, y + half_height,
		    x + half_width, y + height);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + half_width, y + height,
		    x + width, y + half_height);

      gdk_draw_line(window, style->dark_gc[state_type],
		    x + 2, y + half_height,
		    x + half_width, y + 2);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + half_width, y + 2,
		    x + width - 2, y + half_height);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + 1, y + half_height,
		    x + half_width, y + 1);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + half_width, y + 1,
		    x + width - 1, y + half_height);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x, y + half_height,
		    x + half_width, y);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + half_width, y,
		    x + width, y + half_height);
      break;
    case GTK_SHADOW_OUT:
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + 2, y + half_height,
		    x + half_width, y + height - 2);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + half_width, y + height - 2,
		    x + width - 2, y + half_height);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + 1, y + half_height,
		    x + half_width, y + height - 1);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + half_width, y + height - 1,
		    x + width - 1, y + half_height);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x, y + half_height,
		    x + half_width, y + height);
      gdk_draw_line(window, style->dark_gc[state_type],
		    x + half_width, y + height,
		    x + width, y + half_height);

      gdk_draw_line(window, style->light_gc[state_type],
		    x + 2, y + half_height,
		    x + half_width, y + 2);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + half_width, y + 2,
		    x + width - 2, y + half_height);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + 1, y + half_height,
		    x + half_width, y + 1);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + half_width, y + 1,
		    x + width - 1, y + half_height);
      gdk_draw_line(window, style->light_gc[state_type],
		    x, y + half_height,
		    x + half_width, y);
      gdk_draw_line(window, style->light_gc[state_type],
		    x + half_width, y,
		    x + width, y + half_height);
      break;
    default:
      break;
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle(style->light_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle(style->dark_gc[state_type], NULL);
      gdk_gc_set_clip_rectangle(style->black_gc, NULL);
    }
}

static void
draw_oval(GtkStyle * style,
	  GdkWindow * window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle * area,
	  GtkWidget * widget,
	  gchar * detail,
	  gint x,
	  gint y,
	  gint width,
	  gint height)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);
}

static void
draw_string(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    gchar * detail,
	    gint x,
	    gint y,
	    const gchar * string)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if (area)
    {
      gdk_gc_set_clip_rectangle(style->white_gc, area);
      gdk_gc_set_clip_rectangle(style->fg_gc[state_type], area);
    }
  if (state_type == GTK_STATE_INSENSITIVE)
    gdk_draw_string(window, style->font, style->white_gc, x + 1, y + 1, string);
  gdk_draw_string(window, style->font, style->fg_gc[state_type], x, y, string);
  if (area)
    {
      gdk_gc_set_clip_rectangle(style->white_gc, NULL);
      gdk_gc_set_clip_rectangle(style->fg_gc[state_type], NULL);
    }
}

static void
draw_box(GtkStyle * style,
	 GdkWindow * window,
	 GtkStateType state_type,
	 GtkShadowType shadow_type,
	 GdkRectangle * area,
	 GtkWidget * widget,
	 gchar * detail,
	 gint x,
	 gint y,
	 gint width,
	 gint height)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  if ((!style->bg_pixmap[state_type]) ||
      (gdk_window_get_type(window) == GDK_WINDOW_PIXMAP))
    {
      if (area)
	{
	  gdk_gc_set_clip_rectangle(style->bg_gc[state_type], area);
	}
      gdk_draw_rectangle(window, style->bg_gc[state_type], TRUE,
			 x, y, width, height);
      if (area)
	{
	  gdk_gc_set_clip_rectangle(style->bg_gc[state_type], NULL);
	}
    }
  else
    gtk_style_apply_default_pixmap(style, window, state_type, area, x, y, width, height);
  gtk_paint_shadow(style, window, state_type, shadow_type, area, widget, detail,
		   x, y, width, height);
}

static void
draw_flat_box(GtkStyle * style,
	      GdkWindow * window,
	      GtkStateType state_type,
	      GtkShadowType shadow_type,
	      GdkRectangle * area,
	      GtkWidget * widget,
	      gchar * detail,
	      gint x,
	      gint y,
	      gint width,
	      gint height)
{
  GdkGC              *gc1;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  gc1 = style->bg_gc[state_type];

  if ((detail) && (!strcmp("selected", detail)))
    gc1 = style->bg_gc[GTK_STATE_SELECTED];
  if ((detail) && (!strcmp("text", detail)) && (state_type == GTK_STATE_SELECTED))
    gc1 = style->bg_gc[GTK_STATE_SELECTED];
  else if ((detail) && (!strcmp("viewportbin", detail)))
    gc1 = style->bg_gc[GTK_STATE_NORMAL];
  if ((!style->bg_pixmap[state_type]) || (gc1 != style->bg_gc[state_type]) ||
      (gdk_window_get_type(window) == GDK_WINDOW_PIXMAP))
    {
      if (area)
	{
	  gdk_gc_set_clip_rectangle(gc1, area);
	}
      gdk_draw_rectangle(window, gc1, TRUE,
			 x, y, width, height);
      if ((detail) && (!strcmp("tooltip", detail)))
	gdk_draw_rectangle(window, style->black_gc, FALSE,
			   x, y, width - 1, height - 1);
      if (area)
	{
	  gdk_gc_set_clip_rectangle(gc1, NULL);
	}
    }
  else
    gtk_style_apply_default_pixmap(style, window, state_type, area, x, y, width, height);
}

static void
draw_check(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   gchar * detail,
	   gint x,
	   gint y,
	   gint width,
	   gint height)
{
  gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail,
		x, y, width, height);
}

static void
draw_option(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height)
{
  gtk_paint_diamond(style, window, state_type, shadow_type, area, widget,
		    detail, x, y, width, height);
}

static void
draw_cross(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GtkShadowType shadow_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   gchar * detail,
	   gint x,
	   gint y,
	   gint width,
	   gint height)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);
}

static void
draw_ramp(GtkStyle * style,
	  GdkWindow * window,
	  GtkStateType state_type,
	  GtkShadowType shadow_type,
	  GdkRectangle * area,
	  GtkWidget * widget,
	  gchar * detail,
	  GtkArrowType arrow_type,
	  gint x,
	  gint y,
	  gint width,
	  gint height)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);
}

static void
draw_tab(GtkStyle * style,
	 GdkWindow * window,
	 GtkStateType state_type,
	 GtkShadowType shadow_type,
	 GdkRectangle * area,
	 GtkWidget * widget,
	 gchar * detail,
	 gint x,
	 gint y,
	 gint width,
	 gint height)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail,
		x, y, width, height);
}

static void
draw_shadow_gap(GtkStyle * style,
		GdkWindow * window,
		GtkStateType state_type,
		GtkShadowType shadow_type,
		GdkRectangle * area,
		GtkWidget * widget,
		gchar * detail,
		gint x,
		gint y,
		gint width,
		gint height,
		gint gap_side,
		gint gap_x,
		gint gap_width)
{
  GdkRectangle        rect;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  gtk_paint_shadow(style, window, state_type, shadow_type, area, widget, detail,
		   x, y, width, height);

  if (gap_side == 0)
    /* top */
    {
      rect.x = x + gap_x;
      rect.y = y;
      rect.width = gap_width;
      rect.height = 2;
    }
  else if (gap_side == 1)
    /* bottom */
    {
      rect.x = x + gap_x;
      rect.y = y + height - 2;
      rect.width = gap_width;
      rect.height = 2;
    }
  else if (gap_side == 2)
    /* left */
    {
      rect.x = x;
      rect.y = y + gap_x;
      rect.width = 2;
      rect.height = gap_width;
    }
  else if (gap_side == 3)
    /* right */
    {
      rect.x = x + width - 2;
      rect.y = y + gap_x;
      rect.width = 2;
      rect.height = gap_width;
    }

  gtk_style_apply_default_pixmap(style, window, state_type, area,
				 rect.x, rect.y, rect.width, rect.height);
}

static void
draw_box_gap(GtkStyle * style,
	     GdkWindow * window,
	     GtkStateType state_type,
	     GtkShadowType shadow_type,
	     GdkRectangle * area,
	     GtkWidget * widget,
	     gchar * detail,
	     gint x,
	     gint y,
	     gint width,
	     gint height,
	     gint gap_side,
	     gint gap_x,
	     gint gap_width)
{
  GdkRectangle        rect;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail,
		x, y, width, height);

  if (gap_side == 0)
    /* top */
    {
      rect.x = x + gap_x;
      rect.y = y;
      rect.width = gap_width;
      rect.height = 2;
    }
  else if (gap_side == 1)
    /* bottom */
    {
      rect.x = x + gap_x;
      rect.y = y + height - 2;
      rect.width = gap_width;
      rect.height = 2;
    }
  else if (gap_side == 2)
    /* left */
    {
      rect.x = x;
      rect.y = y + gap_x;
      rect.width = 2;
      rect.height = gap_width;
    }
  else if (gap_side == 3)
    /* right */
    {
      rect.x = x + width - 2;
      rect.y = y + gap_x;
      rect.width = 2;
      rect.height = gap_width;
    }

  gtk_style_apply_default_pixmap(style, window, state_type, area,
				 rect.x, rect.y, rect.width, rect.height);
}

static void
draw_extension(GtkStyle * style,
	       GdkWindow * window,
	       GtkStateType state_type,
	       GtkShadowType shadow_type,
	       GdkRectangle * area,
	       GtkWidget * widget,
	       gchar * detail,
	       gint x,
	       gint y,
	       gint width,
	       gint height,
	       gint gap_side)
{
  GdkRectangle        rect;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  gtk_paint_box(style, window, state_type, shadow_type, area, widget, detail,
		x, y, width, height);

  if (gap_side == 0)
    /* top */
    {
      rect.x = x + style->klass->xthickness;
      rect.y = y;
      rect.width = width - style->klass->xthickness * 2;
      rect.height = style->klass->ythickness;
    }
  else if (gap_side == 1)
    /* bottom */
    {
      rect.x = x + style->klass->xthickness;
      rect.y = y + height - style->klass->ythickness;
      rect.width = width - style->klass->xthickness * 2;
      rect.height = style->klass->ythickness;
    }
  else if (gap_side == 2)
    /* left */
    {
      rect.x = x;
      rect.y = y + style->klass->ythickness;
      rect.width = style->klass->xthickness;
      rect.height = height - style->klass->ythickness * 2;
    }
  else if (gap_side == 3)
    /* right */
    {
      rect.x = x + width - style->klass->xthickness;
      rect.y = y + style->klass->ythickness;
      rect.width = style->klass->xthickness;
      rect.height = height - style->klass->ythickness * 2;
    }

  gtk_style_apply_default_pixmap(style, window, state_type, area,
				 rect.x, rect.y, rect.width, rect.height);
}

static void
draw_focus(GtkStyle * style,
	   GdkWindow * window,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   gchar * detail,
	   gint x,
	   gint y,
	   gint width,
	   gint height)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    {
      gdk_window_get_size(window, &width, &height);
      width -= 1;
      height -= 1;
    }
  else if (width == -1)
    {
      gdk_window_get_size(window, &width, NULL);
      width -= 1;
    }
  else if (height == -1)
    {
      gdk_window_get_size(window, NULL, &height);
      height -= 1;
    }
  if (area)
    {
      gdk_gc_set_clip_rectangle(style->black_gc, area);
    }
  gdk_draw_rectangle(window,
		     style->black_gc, FALSE,
		     x, y, width, height);
  if (area)
    {
      gdk_gc_set_clip_rectangle(style->black_gc, NULL);
    }
}

static void
draw_slider(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height,
	    GtkOrientation orientation)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  gtk_draw_box(style, window, state_type, shadow_type, x, y,
	       width, height);
  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    draw_vline(style, window, state_type, area, widget, detail,
	       style->klass->ythickness,
	       height - style->klass->ythickness - 1, width / 2);
  else
    draw_hline(style, window, state_type, area, widget, detail,
	       style->klass->xthickness,
	       width - style->klass->xthickness - 1, height / 2);
}

static void
draw_entry(GtkStyle * style,
	   GdkWindow * window,
	   GtkStateType state_type,
	   GdkRectangle * area,
	   GtkWidget * widget,
	   gchar * detail,
	   gint x,
	   gint y,
	   gint width,
	   gint height)
{
  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  if (area)
    gdk_gc_set_clip_rectangle(style->base_gc[state_type], area);
  if (!strcmp("selected", detail))
    {
      gdk_draw_rectangle(window,
			 style->bg_gc[state_type],
			 TRUE,
			 x, y,
			 width,
			 height);
    }
  else
    {
      if (area)
	gdk_gc_set_clip_rectangle(style->base_gc[state_type], area);
      gdk_draw_rectangle(window,
			 style->base_gc[state_type],
			 TRUE,
			 x, y,
			 width,
			 height);
      if (area)
	gdk_gc_set_clip_rectangle(style->base_gc[state_type], NULL);
    }
}

static void
draw_handle(GtkStyle * style,
	    GdkWindow * window,
	    GtkStateType state_type,
	    GtkShadowType shadow_type,
	    GdkRectangle * area,
	    GtkWidget * widget,
	    gchar * detail,
	    gint x,
	    gint y,
	    gint width,
	    gint height,
	    GtkOrientation orientation)
{
  gint                xx, yy;
  gint                xthick, ythick;
  GdkGC              *light_gc, *dark_gc;
  GdkRectangle        dest;

  g_return_if_fail(style != NULL);
  g_return_if_fail(window != NULL);

  if ((width == -1) && (height == -1))
    gdk_window_get_size(window, &width, &height);
  else if (width == -1)
    gdk_window_get_size(window, &width, NULL);
  else if (height == -1)
    gdk_window_get_size(window, NULL, &height);

  gtk_paint_box(style, window, state_type, shadow_type, area, widget,
		detail, x, y, width, height);

  light_gc = style->light_gc[state_type];
  dark_gc = style->dark_gc[state_type];

  xthick = style->klass->xthickness;
  ythick = style->klass->ythickness;

  dest.x = x + xthick;
  dest.y = y + ythick;
  dest.width = width - (xthick * 2);
  dest.height = height - (ythick * 2);

  gdk_gc_set_clip_rectangle(light_gc, &dest);
  gdk_gc_set_clip_rectangle(dark_gc, &dest);

  for (yy = y + ythick; yy < (y + height - ythick); yy += 3)
    for (xx = x + xthick; xx < (x + width - xthick); xx += 6)
      {
	gdk_draw_point(window, light_gc, xx, yy);
	gdk_draw_point(window, dark_gc, xx + 1, yy + 1);

	gdk_draw_point(window, light_gc, xx + 3, yy + 1);
	gdk_draw_point(window, dark_gc, xx + 4, yy + 2);
      }
  gdk_gc_set_clip_rectangle(light_gc, NULL);
  gdk_gc_set_clip_rectangle(dark_gc, NULL);
}
