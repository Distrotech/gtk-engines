#include "clearlooks_draw.h"
#include "clearlooks_style.h"

#include "support.h"

/** WANTED:
    FASTER GRADIENT FILL FUNCTION, POSSIBLY USING XRENDER. **/

static void cl_draw_borders (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                      int x, int y, int width, int height, CLRectangle *r);

static void cl_draw_line (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                   int x1, int y1, int x2, int y2, CLBorderType border,
                   CLRectangle *r);
				   
static void cl_draw_corner (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                     int x, int y, int width, int height,
                     CLRectangle *r, CLCornerSide corner);
					 
static void cl_draw_fill (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                   int x, int y, int width, int height, CLRectangle *r);

void cl_draw_rectangle (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                        int x, int y, int width, int height, CLRectangle *r)
{
	if (r->fillgc)
	{
		cl_draw_fill(window, widget, style, x, y, width, height, r);
	}
	
	if (r->bordergc)
	{
		cl_draw_borders(window, widget, style, x, y, width, height, r);
	}	
}


static void cl_get_coords ( CLBorderType border,
                     int x, int y, int width, int height,
                     CLRectangle *r, int *x1, int *y1, int *x2, int *y2)
{
	switch (border)
	{
		case CL_BORDER_TOP:
			*x1 = x + r->corners[CL_CORNER_TOPLEFT];
			*x2 = *x1 + width - r->corners[CL_CORNER_TOPLEFT] - r->corners[CL_CORNER_TOPRIGHT] - 1;
			*y1 = *y2 = y;
			break;
		case CL_BORDER_BOTTOM:
			*x1 = x + r->corners[CL_CORNER_BOTTOMLEFT];
			*x2 = *x1 + width - r->corners[CL_CORNER_BOTTOMLEFT] - r->corners[CL_CORNER_BOTTOMRIGHT] - 1;
			*y1 = *y2 = y + height - 1;
			break;
		case CL_BORDER_LEFT:
			*x1 = *x2 = x;
			*y1 = y + r->corners[CL_CORNER_TOPLEFT];
			*y2 = *y1 + height - r->corners[CL_CORNER_TOPLEFT] - r->corners[CL_CORNER_BOTTOMLEFT] - 1;
			break;
		case CL_BORDER_RIGHT:
			*x1 = *x2 = x + width - 1;
			*y1 = y + r->corners[CL_CORNER_TOPRIGHT];
			*y2 = *y1 + height - r->corners[CL_CORNER_TOPRIGHT] - r->corners[CL_CORNER_BOTTOMRIGHT] - 1;
			break;
	}
}

void cl_draw_borders (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                      int x, int y, int width, int height, CLRectangle *r)
{
	int x1, y1, x2, y2, i;

	if (r->bordergc == NULL)
		return;

	for ( i=0; i<4; i++) /* draw all four borders + corners */
	{
		cl_get_coords (i, x, y, width, height, r, &x1, &y1, &x2, &y2);
		cl_draw_line (window, widget, style, x1, y1, x2, y2, i, r);
		cl_draw_corner (window, widget, style, x, y, width, height, r, i );
	}
}


static GdkColor cl_gc_get_foreground(GdkGC *gc)
{
	GdkGCValues values;
	gdk_gc_get_values (gc, &values);
	return values.foreground;
}

static void cl_draw_line (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                   int x1, int y1, int x2, int y2, CLBorderType border,
                   CLRectangle *r)
{
	if (r->gradient_type == CL_GRADIENT_NONE ||
		r->border_gradient.from == NULL || r->border_gradient.to == NULL )
	{
		gdk_draw_line (window, r->bordergc, x1, y1, x2, y2);
	}
	else if (r->gradient_type == CL_GRADIENT_HORIZONTAL && (border == CL_BORDER_TOP || border == CL_BORDER_BOTTOM))
	{
		cl_draw_vgradient (window, r->bordergc, style,
		                x1, y1, x2-x1+1, 1,
		                r->border_gradient.from, r->border_gradient.to);
	}
	else if (r->gradient_type == CL_GRADIENT_VERTICAL && (border == CL_BORDER_LEFT || border == CL_BORDER_RIGHT))
	{
		cl_draw_hgradient (window, r->bordergc, style,
		                x1, y1, 1, y2-y1+1,
		                r->border_gradient.from, r->border_gradient.to);
	}
	else
	{
		GdkColor tmp_color = cl_gc_get_foreground (r->bordergc);

		if ((r->gradient_type == CL_GRADIENT_HORIZONTAL && border == CL_BORDER_LEFT) ||
			(r->gradient_type == CL_GRADIENT_VERTICAL && border == CL_BORDER_TOP))
			gdk_gc_set_foreground (r->bordergc, r->border_gradient.from);
		else
			gdk_gc_set_foreground (r->bordergc, r->border_gradient.to);				

		gdk_draw_line (window, r->bordergc, x1, y1, x2, y2);
		
		gdk_gc_set_foreground (r->bordergc, &tmp_color);
	}
}

static GdkColor *cl_get_gradient_corner_color (CLRectangle *r, CLCornerSide corner)
{
	GdkColor *color;

	if (r->border_gradient.from == NULL || r->border_gradient.to == NULL)
	{
		color = NULL;
	}
	else if ((r->gradient_type == CL_GRADIENT_HORIZONTAL && (corner == CL_CORNER_TOPLEFT || corner == CL_CORNER_BOTTOMLEFT)) ||
	    (r->gradient_type == CL_GRADIENT_VERTICAL && (corner == CL_CORNER_TOPLEFT || corner == CL_CORNER_TOPRIGHT)))
	{
		color = r->border_gradient.from;
	}
	else /* no gradient or other corner */
	{
		color = r->border_gradient.to;
	}
	
	return color;
}

static void cl_draw_corner (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                     int x, int y, int width, int height,
                     CLRectangle *r, CLCornerSide corner)
{
	GdkColor    *color;
	GdkColor     aacolor; /* anti-aliasing color */
	GdkGCValues  values;
	GdkColor     tmp;
	GdkColor    *bgcolor;

	int x1;
	int y1;

	if (r->corners[corner] == CL_CORNER_NONE)
		return;
	
	color = cl_get_gradient_corner_color (r, corner);
	gdk_gc_get_values (r->bordergc, &values);

	if (color == NULL)
	{
		tmp = values.foreground;
		gdk_colormap_query_color (gtk_widget_get_colormap(widget), values.foreground.pixel, &tmp);
		color = &tmp;
	}

	bgcolor = cl_get_parent_bgcolor(widget);

	if (bgcolor == NULL)
	{
		bgcolor = color;
	}

	cl_blend (style->colormap, bgcolor, color, &aacolor, 70);

	if (r->corners[corner] == CL_CORNER_ROUND)
	{
		x1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_BOTTOMLEFT) ? x+1 : x+width - 2;
		
		y1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_TOPRIGHT) ? y+1 : y+height - 2;
		
		gdk_gc_set_foreground (r->bordergc, color);
		gdk_draw_point (window, r->bordergc, x1, y1);
		
		gdk_gc_set_foreground (r->bordergc, &aacolor);
		
		x1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_BOTTOMLEFT) ? x+1 : x+width-2;

		y1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_TOPRIGHT) ? y : y+height-1;		
		
		gdk_draw_point (window, r->bordergc, x1, y1);

		x1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_BOTTOMLEFT) ? x : x+width-1;

		y1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_TOPRIGHT) ? y+1 : y+height-2;

		gdk_draw_point (window, r->bordergc, x1, y1);
								
	}
	else if (r->corners[corner] == CL_CORNER_NARROW)
	{
		x1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_BOTTOMLEFT) ? x : x+width-1;

		y1 = (corner == CL_CORNER_TOPLEFT ||
		      corner == CL_CORNER_TOPRIGHT) ? y : y+height-1;
				
		gdk_gc_set_foreground (r->bordergc, &aacolor);
		gdk_draw_point (window, r->bordergc, x1, y1);
 	}

	gdk_gc_set_foreground (r->bordergc, &values.foreground);
}

static void cl_draw_fill (GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                   int x, int y, int width, int height, CLRectangle *r)
{
	if (r->gradient_type == CL_GRADIENT_NONE ||
		r->fill_gradient.from == NULL || r->fill_gradient.to == NULL)
	{
		gdk_draw_rectangle (window, r->fillgc, TRUE,
		                    x+1, y+1, width-2, height-2);
	}
	else if (r->gradient_type == CL_GRADIENT_HORIZONTAL)
	{
		cl_draw_vgradient (window, r->fillgc, gtk_widget_get_style(widget),
		                x+1, y+1, width-2, height-2,
		                r->fill_gradient.from, r->fill_gradient.to);
	}
	else if (r->gradient_type == CL_GRADIENT_VERTICAL)
	{
		cl_draw_hgradient (window, r->fillgc, gtk_widget_get_style(widget),
		                x+1, y+1, width-2, height-2,
		                r->fill_gradient.from, r->fill_gradient.to);
	}
}

void cl_rectangle_set_button(CLRectangle *r, GtkStyle *style,
                            GtkStateType state_type,  gboolean has_default,
                            gboolean has_focus,
                            CLBorderType tl, CLBorderType tr,
                            CLBorderType bl, CLBorderType br)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	int              my_state_type = (state_type == GTK_STATE_ACTIVE) ? 2 : 0;
	
	cl_rectangle_init (r, style->bg_gc[state_type],
	                   clearlooks_style->border_gc[CL_BORDER_UPPER+my_state_type],
	                   tl, tr, bl, br);

	if (state_type != GTK_STATE_INSENSITIVE && !has_default)
	{
		cl_rectangle_set_gradient (&r->border_gradient,
		                           &clearlooks_style->border[CL_BORDER_UPPER+my_state_type],
		                           &clearlooks_style->border[CL_BORDER_LOWER+my_state_type]);
	}
	else if (has_default)
		r->bordergc = style->black_gc;
	else
		r->bordergc = clearlooks_style->shade_gc[4];

	r->gradient_type = CL_GRADIENT_VERTICAL;

	r->topleft     = (state_type != GTK_STATE_ACTIVE) ? style->light_gc[state_type] : clearlooks_style->shade_gc[4];
	r->bottomright = (state_type != GTK_STATE_ACTIVE) ? clearlooks_style->shade_gc[1] : NULL;
	
	cl_shade (&style->bg[state_type], &r->tmp_color, 0.93);
	

	cl_rectangle_set_gradient (&r->fill_gradient,
	                           &style->bg[state_type],
	                           &r->tmp_color);
}

void cl_rectangle_set_entry (CLRectangle *r, GtkStyle *style,
                            GtkStateType state_type,
                            CLBorderType tl, CLBorderType tr,
                            CLBorderType bl, CLBorderType br,
                            gboolean has_focus)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	GdkGC *bordergc;
	
	if (has_focus)
		bordergc = clearlooks_style->spot3_gc;
	else if (state_type != GTK_STATE_INSENSITIVE)
		bordergc = clearlooks_style->border_gc[CL_BORDER_LOWER];
	else
		bordergc = clearlooks_style->shade_gc[3];		
	
	cl_rectangle_init (r, style->base_gc[state_type], bordergc,
    	               tl, tr, bl, br);

	if (state_type != GTK_STATE_INSENSITIVE )
		r->topleft     = (has_focus) ? clearlooks_style->spot1_gc
	                                 : style->bg_gc[GTK_STATE_NORMAL];
	
	if (has_focus)
		r->bottomright = clearlooks_style->spot1_gc;
	else if (state_type == GTK_STATE_INSENSITIVE)
		r->bottomright = style->base_gc[state_type];
}

void cl_draw_shadow(GdkWindow *window, GtkWidget *widget, GtkStyle *style,
                    int x, int y, int width, int height, CLRectangle *r)
{
	int x1, y1, x2, y2;

	if (r->bottomright != NULL)
	{
		x1 = x+1+(r->corners[CL_CORNER_BOTTOMLEFT]/2);
		y1 = y2 = y+height-2;
		x2 = x+width - 1 - (1+r->corners[CL_CORNER_BOTTOMRIGHT]/2);
		
		gdk_draw_line (window, r->bottomright, x1, y1, x2, y2);
		
		x1 = x2 = x+width-2;
		y1 = y+1+(r->corners[CL_CORNER_TOPRIGHT]/2);
		y2 = y+height - 1 - (1+r->corners[CL_CORNER_BOTTOMRIGHT]/2);

		gdk_draw_line (window, r->bottomright, x1, y1, x2, y2);
	}
	
	if (r->topleft != NULL)
	{
		x1 = x+1+(r->corners[CL_CORNER_TOPLEFT]/2);
		y1 = y2 = y+1;
		x2 = x+width-1-(1+r->corners[CL_CORNER_TOPRIGHT]/2);
		
		gdk_draw_line (window, r->topleft, x1, y1, x2, y2);
		
		x1 = x2 = x+1;
		y1 = y+1+(r->corners[CL_CORNER_TOPLEFT]/2);
		y2 = y+height-1-(1+r->corners[CL_CORNER_BOTTOMLEFT]/2);

		gdk_draw_line (window, r->topleft, x1, y1, x2, y2);
	}
}

void cl_rectangle_set_color (CLGradient *g, GdkColor *color)
{
	g->from = color;
	g->to   = color;
}

void cl_rectangle_set_gradient (CLGradient *g, GdkColor *from, GdkColor *to)
{
	g->from = from;
	g->to   = to;
}

void cl_rectangle_init (CLRectangle *r,
                        GdkGC *fillgc, GdkGC *bordergc,
                        int tl, int tr, int bl, int br)
{
	r->gradient_type = CL_GRADIENT_NONE;
	
	r->border_gradient.from = r->border_gradient.to = NULL;
	r->fill_gradient.from   = r->fill_gradient.to   = NULL;
	
	r->fillgc      = fillgc;
	r->bordergc    = bordergc;
	
	r->topleft     = NULL;
	r->bottomright = NULL;
	
	r->corners[CL_CORNER_TOPLEFT] = tl;
	r->corners[CL_CORNER_TOPRIGHT] = tr;
	r->corners[CL_CORNER_BOTTOMLEFT] = bl;
	r->corners[CL_CORNER_BOTTOMRIGHT] = br;
}

void cl_rectangle_set_corners (CLRectangle *r, int tl, int tr, int bl, int br)
{
	r->corners[CL_CORNER_TOPLEFT] = tl;
	r->corners[CL_CORNER_TOPRIGHT] = tr;
	r->corners[CL_CORNER_BOTTOMLEFT] = bl;
	r->corners[CL_CORNER_BOTTOMRIGHT] = br;	
}

void cl_set_corner_sharpness (const gchar *detail, GtkWidget *widget, CLRectangle *r)
{
	if (widget && ((widget->parent && GTK_IS_COMBO_BOX_ENTRY (widget->parent)) || GTK_IS_COMBO (widget->parent)))
	{
		gboolean rtl = cl_get_parent_direction (widget) == GTK_TEXT_DIR_RTL;
		int cl = rtl ? CL_CORNER_ROUND : CL_CORNER_NONE;
		int cr = rtl ? CL_CORNER_NONE  : CL_CORNER_ROUND;
		
		cl_rectangle_set_corners (r, cl, cr, cl, cr);
	}
	else if (detail && !strcmp (detail, "spinbutton_up"))
	{
		gboolean rtl = cl_get_parent_direction (widget) == GTK_TEXT_DIR_RTL;
		int tl = rtl ? CL_CORNER_ROUND : CL_CORNER_NONE;
		int tr = rtl ? CL_CORNER_NONE  : CL_CORNER_ROUND;

		cl_rectangle_set_corners (r, tl, tr,
		                          CL_CORNER_NONE, CL_CORNER_NONE);
	}
	else if (detail && !strcmp (detail, "spinbutton_down"))
	{
		gboolean rtl = cl_get_parent_direction (widget) == GTK_TEXT_DIR_RTL;
		int bl = rtl ? CL_CORNER_ROUND : CL_CORNER_NONE;
		int br = rtl ? CL_CORNER_NONE  : CL_CORNER_ROUND;

		cl_rectangle_set_corners (r, CL_CORNER_NONE, CL_CORNER_NONE,
		                          bl, br);
	}
	else
	{
		cl_rectangle_set_corners (r, CL_CORNER_ROUND, CL_CORNER_ROUND,
		                          CL_CORNER_ROUND, CL_CORNER_ROUND);
	};
}

void cl_rectangle_set_clip_rectangle (CLRectangle *r, GdkRectangle *area)
{
	if (area == NULL)
		return;
	
	if (r->fillgc)
		gdk_gc_set_clip_rectangle (r->fillgc, area);
	
	if (r->bordergc)
		gdk_gc_set_clip_rectangle (r->bordergc, area);		

	if (r->topleft)
		gdk_gc_set_clip_rectangle (r->topleft, area);		

	if (r->bottomright)
		gdk_gc_set_clip_rectangle (r->bottomright, area);		
}

void cl_rectangle_reset_clip_rectangle (CLRectangle *r)
{
	if (r->fillgc)
		gdk_gc_set_clip_rectangle (r->fillgc, NULL);
	
	if (r->bordergc)
		gdk_gc_set_clip_rectangle (r->bordergc, NULL);

	if (r->topleft)
		gdk_gc_set_clip_rectangle (r->topleft, NULL);

	if (r->bottomright)
		gdk_gc_set_clip_rectangle (r->bottomright, NULL);
}

void cl_rectangle_reset (CLRectangle *r, GtkStyle *style)
{
	cl_rectangle_init (r,
	                   NULL, NULL,
	                   CL_CORNER_ROUND, CL_CORNER_ROUND,
	                   CL_CORNER_ROUND, CL_CORNER_ROUND);
}

GdkColor cl_gc_set_fg_color_shade (GdkGC *gc, GdkColormap *colormap, 
                                   GdkColor *from, gfloat s)
{
	GdkColor tmp_color;
	GdkGCValues values;

	cl_shade (from, &tmp_color, s);
	gdk_gc_get_values (gc, &values);
	gdk_rgb_find_color (colormap, &tmp_color);
	gdk_gc_set_foreground (gc, &tmp_color);
	
	return values.foreground;
}

/* #warning MOVE THIS TO SUPPORT.C/H SO THE DRAW_CORNER FUNCTION CAN USE IT. OR, MAKE DRAW_CORNER USE IT SOME OTHER WAY. */

static void cl_get_window_style_state (GtkWidget *widget, GtkStyle **style, GtkStateType *state_type)
{
	GtkWidget *tmpwidget = widget;
	
	if (widget && GTK_IS_ENTRY (widget))
		tmpwidget = tmpwidget->parent;
	
	while (tmpwidget && GTK_WIDGET_NO_WINDOW (tmpwidget) && !GTK_IS_NOTEBOOK(tmpwidget))
	{
		tmpwidget = tmpwidget->parent;
	}

	*style     = tmpwidget->style;
	*state_type = GTK_WIDGET_STATE(tmpwidget);
}

static GdkGC *cl_get_window_bg_gc (GtkWidget *widget, GtkStyle *fallback)
{
	GtkStyle *style = fallback;
	GtkStateType state_type;
	
	cl_get_window_style_state (widget, &style, &state_type);
	
	return style->bg_gc[state_type];
}

/******************************************************************************
 *   DRAW THE MIGHTY WIDGETS!                                                 *
 ******************************************************************************/

void cl_draw_inset (GtkStyle *style, GdkWindow *window, GtkWidget *widget,
                    GdkRectangle *area,
                    gint x, gint y, gint width, gint height,
                    int tl, int tr, int bl, int br )
{
	ClearlooksStyle *clwindowstyle; /* style of the window this widget is on */
	GtkStateType     windowstate;	
	CLRectangle      r;

	cl_rectangle_init (&r, NULL, style->black_gc,
	                   tl, tr, bl, br);
	
	r.gradient_type = CL_GRADIENT_VERTICAL;
	
	cl_get_window_style_state(widget, (GtkStyle**)&clwindowstyle, &windowstate);
	
	g_assert (clwindowstyle != NULL);
	
	if (widget && GTK_WIDGET_HAS_DEFAULT (widget))
	{
		r.bordergc = style->mid_gc[GTK_STATE_NORMAL];
	}
	else
	{
		cl_rectangle_set_gradient (&r.border_gradient,
		                           &clwindowstyle->inset_dark[windowstate],
		                           &clwindowstyle->inset_light[windowstate]);
	}
	cl_rectangle_set_clip_rectangle (&r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, &r); 
	cl_rectangle_reset_clip_rectangle (&r);
}

/* Draw a normal (toggle)button. Not spinbuttons.*/ 
void cl_draw_button(GtkStyle *style, GdkWindow *window,
                    GtkStateType state_type, GtkShadowType shadow_type,
                    GdkRectangle *area,
                    GtkWidget *widget, const gchar *detail,
                    gint x, gint y, gint width, gint height)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE(style);
	GdkGC *bg_gc = NULL;
	gboolean is_active = FALSE;
	CLRectangle r;
	
	/* Get the background color of the window we're on */
	bg_gc = cl_get_window_bg_gc(widget, style);
	
	cl_rectangle_set_button (&r, style, state_type,
	                         widget && GTK_WIDGET_HAS_DEFAULT (widget),
	                         widget && GTK_WIDGET_HAS_FOCUS (widget),
	                         CL_CORNER_ROUND, CL_CORNER_ROUND,
	                         CL_CORNER_ROUND, CL_CORNER_ROUND);
		
	if (state_type == GTK_STATE_ACTIVE)
		is_active = TRUE;

	if (GTK_IS_TOGGLE_BUTTON(widget) &&
	    gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(widget)) &&
	    state_type == GTK_STATE_PRELIGHT)
	{
		cl_rectangle_set_gradient (&r.fill_gradient, &clearlooks_style->cl_shade[1], &clearlooks_style->cl_shade[1]);
		r.topleft     = clearlooks_style->shade_gc[3];
		r.bottomright = clearlooks_style->shade_gc[1];
		
		is_active = TRUE;
	}	

	if (!is_active)
		r.fillgc = NULL;
		
	if (!widget || !GTK_IS_NOTEBOOK (widget->parent))
	{
		gdk_draw_rectangle (window, bg_gc, FALSE, x, y, width-1, height-1);
	
		/* Draw "sunken" look when border thickness is more than 2 pixels. */
		if (style->xthickness > 2 && style->ythickness > 2)
		cl_draw_inset (style, window, widget, area, x, y, width, height,
		               CL_CORNER_ROUND, CL_CORNER_ROUND,
		               CL_CORNER_ROUND, CL_CORNER_ROUND);
	}
	
	/* Draw "sunken" look when border thickness is more than 2 pixels.*/
	if (style->xthickness > 2 && style->ythickness > 2)
	{
		x++;
		y++;
		height-=2;
		width-=2;
	}
	
	/* Don't draw the normal gradient for normal buttons. */

	cl_rectangle_set_clip_rectangle (&r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	
	
	if (!is_active) 
	{
		int tmp_height = (float)height*0.25;
	
		gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
		
		cl_draw_hgradient (window, style->bg_gc[state_type], style,
		                x+2,y+2,width-4,tmp_height,
		                &clearlooks_style->button_g1[state_type],
			            &clearlooks_style->button_g2[state_type]);
		
		cl_draw_hgradient (window, style->bg_gc[state_type], style,
		                x+2, y+2+tmp_height, width-4, height-3-tmp_height*2,
		                &clearlooks_style->button_g2[state_type],
			            &clearlooks_style->button_g3[state_type]);
		
		cl_draw_hgradient (window, style->bg_gc[state_type], style,
		                x+2,y+height-tmp_height-1,width-4,tmp_height,
		                &clearlooks_style->button_g3[state_type],
			            &clearlooks_style->button_g4[state_type]);

		gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
	}				
	
	cl_draw_shadow    (window, widget, style, x, y, width, height, &r);
	cl_rectangle_reset_clip_rectangle (&r);
}

/* Draw spinbuttons. */
void cl_draw_spinbutton(GtkStyle *style, GdkWindow *window,
                        GtkStateType state_type, GtkShadowType shadow_type,
                        GdkRectangle *area,
                        GtkWidget *widget, const gchar *detail,
                        gint x, gint y, gint width, gint height)
{
	CLRectangle r;
	GdkRectangle new_area;

	int tl = CL_CORNER_NONE, tr = CL_CORNER_NONE,
	    bl = CL_CORNER_NONE, br = CL_CORNER_NONE;	
	
	if (area == NULL)
	{
		new_area.x = x;
		new_area.y = y;
		new_area.width = width;
		new_area.height = height;
		area = &new_area;		
	}

	if (detail && !strcmp (detail, "spinbutton")) /* draws the 'back' of the spinbutton */
	{
		GdkGC *bg_gc = cl_get_window_bg_gc(widget, style);
		
		gdk_gc_set_clip_rectangle (bg_gc, area);
		gdk_draw_rectangle (window, bg_gc, FALSE, x, y, width-1, height-1);
		gdk_gc_set_clip_rectangle (bg_gc, NULL);

		if (style->xthickness > 2 && style->ythickness > 2)
			cl_draw_inset (style, window, widget, area, x, y, width, height,
			               CL_CORNER_NONE, CL_CORNER_ROUND,
			               CL_CORNER_NONE, CL_CORNER_ROUND);
		
		return;
	}

	if (detail && !strcmp (detail, "spinbutton_up"))
	{
		tr = CL_CORNER_ROUND;
		
		(style->xthickness > 2 && style->ythickness > 2) ? y++ : height++;
	}
	
	if (!strcmp (detail, "spinbutton_down"))
	{
		br = CL_CORNER_ROUND;
		
		if (style->xthickness > 2 && style->ythickness > 2)
			height--;
	}
	
	cl_rectangle_set_button (&r, style, state_type,
	                         widget && GTK_WIDGET_HAS_DEFAULT (widget),
	                         widget && GTK_WIDGET_HAS_FOCUS (widget),
	                         tl, tr,
	                         bl, br);
	width--;
	
	cl_rectangle_set_clip_rectangle (&r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	cl_draw_shadow    (window, widget, style, x, y, width, height, &r);
	cl_rectangle_reset_clip_rectangle (&r);
}

void cl_draw_combobox_entry (GtkStyle *style, GdkWindow *window,
                             GtkStateType state_type, GtkShadowType shadow_type,
                             GdkRectangle *area,
                             GtkWidget *widget, const gchar *detail,
                             gint x, gint y, gint width, gint height)
{
	CLRectangle r;
	
	gboolean rtl;
	gboolean has_focus = widget && GTK_WIDGET_HAS_FOCUS (widget);
	
	int cl, cr;
	
	rtl = cl_get_parent_direction (widget) == GTK_TEXT_DIR_RTL;
	
	cl = rtl ? CL_CORNER_NONE  : CL_CORNER_ROUND;
	cr = rtl ? CL_CORNER_ROUND : CL_CORNER_NONE;
	
	GdkGC *bg_gc = cl_get_window_bg_gc(widget, style);
	
	if (rtl)
	{
		if (!has_focus)
		{
			x -= 1;
			width +=1;
		}
	}
	else
	{
		width += 2;
		if (has_focus) width--; /* this gives us a 2px focus line at the right side. */
	}
	
	cl_rectangle_set_entry (&r, style, state_type,
						   cl, cr, cl, cr,
						   has_focus);

	gdk_gc_set_clip_rectangle (bg_gc, area);
	gdk_draw_rectangle (window, bg_gc, FALSE, x, y, width-1, height-1);
	gdk_gc_set_clip_rectangle (bg_gc, NULL);

	/* Draw "sunken" look when border thickness is more than 2 pixels. */
	if (style->xthickness > 2 && style->ythickness > 2)
	{
		cl_draw_inset (style, window, widget, area, x, y, width, height,
		               cl, cr, cl, cr);

		y++;
		x++;
		width-=2;
		height-=2;
	}
	
	cl_rectangle_set_clip_rectangle (&r, area);

	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	cl_draw_shadow (window, widget, style, x, y, width, height, &r); 
	
	cl_rectangle_reset_clip_rectangle (&r);
}

void cl_draw_combobox_button (GtkStyle *style, GdkWindow *window,
                    GtkStateType state_type, GtkShadowType shadow_type,
                    GdkRectangle *area,
                    GtkWidget *widget, const gchar *detail,
                    gint x, gint y, gint width, gint height)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE(style);
	gboolean is_active  = FALSE;
	gboolean draw_inset = FALSE;
	CLRectangle r;

	cl_rectangle_set_button (&r, style, state_type,
	                         GTK_WIDGET_HAS_DEFAULT  (widget),
	                         GTK_WIDGET_HAS_FOCUS (widget),
	                         CL_CORNER_NONE, CL_CORNER_ROUND,
	                         CL_CORNER_NONE, CL_CORNER_ROUND);
	
	if (state_type == GTK_STATE_ACTIVE)
		is_active = TRUE;
	else
		r.fillgc = NULL;
	
	/* Seriously, why can't non-gtk-apps at least try to be decent citizens?
	   Take this fscking OpenOffice.org 1.9 for example. The morons responsible
	   for this utter piece of crap give the clip size wrong values! :'(  */
	
	if (area)
	{
		area->x = x;
		area->y = y;
		area->width = width;
		area->height = height;
	}

	x--;
	width++;
	
	/* Draw "sunken" look when border thickness is more than 2 pixels. */
	if (GTK_IS_COMBO(widget->parent))
		draw_inset = (widget->parent->style->xthickness > 2 &&
	                  widget->parent->style->ythickness > 2);
	else
		draw_inset = (style->xthickness > 2 && style->ythickness > 2);
		
	if (draw_inset)
	{
		cl_draw_inset (style, window, widget, area, x, y, width, height,
		               CL_CORNER_NONE, CL_CORNER_ROUND,
		               CL_CORNER_NONE, CL_CORNER_ROUND);
	
		x++;
		y++;
		height-=2;
		width-=2;
	}
	else
	{
		x++;
		width--;
	}
	
	if (area)
		cl_rectangle_set_clip_rectangle (&r, area);

	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	
	if (!is_active)
	{
		int tmp_height = (float)height*0.25;
	
		gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);
		
		cl_draw_hgradient (window, style->bg_gc[state_type], style,
		                x+2,y+2,width-4,tmp_height,
		                &clearlooks_style->button_g1[state_type],
			            &clearlooks_style->button_g2[state_type]);
		
		cl_draw_hgradient (window, style->bg_gc[state_type], style,
		                x+2, y+2+tmp_height, width-4, height-3-tmp_height*2,
		                &clearlooks_style->button_g2[state_type],
			            &clearlooks_style->button_g3[state_type]);
		
		cl_draw_hgradient (window, style->bg_gc[state_type], style,
		                x+2,y+height-tmp_height-1,width-4,tmp_height,
		                &clearlooks_style->button_g3[state_type],
			            &clearlooks_style->button_g4[state_type]);

		gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
	}					
	
	cl_draw_shadow    (window, widget, style, x, y, width, height, &r);

	if (area)
		cl_rectangle_reset_clip_rectangle (&r);
}

/* Draw text Entry */
void cl_draw_entry (GtkStyle *style, GdkWindow *window,
                        GtkStateType state_type, GtkShadowType shadow_type,
                        GdkRectangle *area,
                        GtkWidget *widget, const gchar *detail,
                        gint x, gint y, gint width, gint height)
{
	CLRectangle r;
	gboolean has_focus = widget && GTK_WIDGET_HAS_FOCUS(widget);
	GdkGC *bg_gc = cl_get_window_bg_gc(widget, style);
	
	gdk_draw_rectangle (window, bg_gc, FALSE, x, y, width-1, height-1);

	gtk_style_apply_default_background (style, window, TRUE, state_type,
										area, x+1, y+1, width-2, height-2);

	
	cl_rectangle_set_entry (&r, style, state_type,
							CL_CORNER_ROUND, CL_CORNER_ROUND,
							CL_CORNER_ROUND, CL_CORNER_ROUND,
							has_focus);
	
	/* Draw "sunken" look when border thickness is more than 2 pixels. */
	if (style->xthickness > 2 && style->ythickness > 2)
	{
		cl_draw_inset (style, window, widget, area, x, y, width, height,
		               CL_CORNER_ROUND, CL_CORNER_ROUND,
		               CL_CORNER_ROUND, CL_CORNER_ROUND);
	
		x++;
		y++;
		width-=2;
		height-=2;
	}
	
	cl_rectangle_set_clip_rectangle (&r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	cl_draw_shadow (window, widget, style, x, y, width, height, &r); 
	cl_rectangle_reset_clip_rectangle (&r);
}

void cl_draw_optionmenu(GtkStyle *style, GdkWindow *window,
                        GtkStateType state_type, GtkShadowType shadow_type,
                        GdkRectangle *area, GtkWidget *widget,
                        const gchar *detail,
                        gint x, gint y, gint width, gint height)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE(style);
	GtkRequisition indicator_size;
	GtkBorder indicator_spacing;
	int line_pos;

	cl_option_menu_get_props (widget, &indicator_size, &indicator_spacing);
	
	if (cl_get_direction (widget) == GTK_TEXT_DIR_RTL)
		line_pos = x + (indicator_size.width + indicator_spacing.left + indicator_spacing.right) + style->xthickness;
	else
		line_pos = x + width - (indicator_size.width + indicator_spacing.left + indicator_spacing.right) - style->xthickness;

	cl_draw_button (style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
	
	gdk_draw_line (window, clearlooks_style->shade_gc[3],
				   line_pos, y + style->ythickness - 1, line_pos,
				   y + height - style->ythickness);

	gdk_draw_line (window, style->light_gc[state_type],
				   line_pos+1, y + style->ythickness - 1, line_pos+1,
				   y + height - style->ythickness);
}


void cl_draw_menuitem_button (GdkDrawable *window, GtkWidget *widget, GtkStyle *style,
                              GdkRectangle *area, GtkStateType state_type, 
                              int x, int y, int width, int height, CLRectangle *r)
{
	ClearlooksStyle *clearlooks_style = (ClearlooksStyle*)style;
	gboolean menubar  = (widget && widget->parent && GTK_IS_MENU_BAR(widget->parent)) ? TRUE : FALSE;
	int corner        = CL_CORNER_NARROW;
	GdkColor lower_color;

	cl_shade (&style->base[GTK_STATE_SELECTED], &lower_color, 0.85);
	
	if (menubar)
	{
		height++;
		corner = CL_CORNER_NONE;
		r->bordergc    = clearlooks_style->border_gc[CL_BORDER_UPPER];
	}
	else
	{
		r->bordergc    = clearlooks_style->spot3_gc;
	}
	
	cl_rectangle_set_corners (r, corner, corner, corner, corner);
	
	cl_rectangle_set_gradient (&r->fill_gradient,
	                           &style->base[GTK_STATE_SELECTED], &lower_color);

	r->gradient_type = CL_GRADIENT_VERTICAL;
	
	r->fillgc  = clearlooks_style->spot2_gc;
	r->topleft = clearlooks_style->spot1_gc;
	
	cl_rectangle_set_clip_rectangle (r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, r);
	cl_draw_shadow (window, widget, style, x, y, width, height, r);
	cl_rectangle_reset_clip_rectangle (r);
}

void cl_draw_menuitem_flat (GdkDrawable *window, GtkWidget *widget, GtkStyle *style,
                              GdkRectangle *area, GtkStateType state_type, 
                              int x, int y, int width, int height, CLRectangle *r)
{
	gboolean menubar  = (widget && widget->parent && GTK_IS_MENU_BAR(widget->parent)) ? TRUE : FALSE;
	GdkColor tmp;
	
	cl_rectangle_set_corners (r, CL_CORNER_NARROW, CL_CORNER_NARROW,
	                             CL_CORNER_NARROW, CL_CORNER_NARROW);
	
	tmp = cl_gc_set_fg_color_shade (style->black_gc, style->colormap,
	                                &style->base[GTK_STATE_PRELIGHT], 0.8);

	r->bordergc = style->black_gc;
	r->fillgc = style->base_gc[GTK_STATE_PRELIGHT];
	
	if (menubar) height++;

	cl_rectangle_set_clip_rectangle (r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, r);
	cl_rectangle_reset_clip_rectangle (r);	
	
	gdk_gc_set_foreground (style->black_gc, &tmp);
}

void cl_draw_menuitem_gradient (GdkDrawable *window, GtkWidget *widget, GtkStyle *style,
                                GdkRectangle *area, GtkStateType state_type, 
                                int x, int y, int width, int height, CLRectangle *r)
{
	gboolean menubar  = (widget && widget->parent && GTK_IS_MENU_BAR(widget->parent)) ? TRUE : FALSE;
	GdkColor tmp;
	GdkColor lower_color;
	
	cl_shade (&style->base[GTK_STATE_SELECTED], &lower_color, 0.8);
	
	cl_rectangle_set_corners (r, CL_CORNER_NARROW, CL_CORNER_NARROW,
	                             CL_CORNER_NARROW, CL_CORNER_NARROW);
	                             
	cl_rectangle_set_gradient (&r->fill_gradient,
	                           &style->base[GTK_STATE_SELECTED], &lower_color);

	r->gradient_type = CL_GRADIENT_VERTICAL;
	
	tmp = cl_gc_set_fg_color_shade (style->black_gc, style->colormap,
	                                &style->base[GTK_STATE_PRELIGHT], 0.8);

	r->bordergc = style->black_gc;
	r->fillgc = style->base_gc[GTK_STATE_PRELIGHT];
	
	if (menubar) height++;

	cl_rectangle_set_clip_rectangle (r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, r);
	cl_rectangle_reset_clip_rectangle (r);
	
	gdk_gc_set_foreground (style->black_gc, &tmp);
}

void cl_draw_treeview_header (GtkStyle *style, GdkWindow *window,
                              GtkStateType state_type, GtkShadowType shadow_type,
                              GdkRectangle *area,
                              GtkWidget *widget, const gchar *detail,
                              gint x, gint y, gint width, gint height)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	gint columns = 0, column_index = -1, fill_width = width;
	gboolean resizable = TRUE;
	
	GdkGC *bottom = clearlooks_style->shade_gc[5];
		
	if ( width < 2 || height < 2 )
		return;
	
	if (widget && GTK_IS_TREE_VIEW (widget->parent))
	{
		gtk_treeview_get_header_index (GTK_TREE_VIEW(widget->parent),
	                                   widget, &column_index, &columns,
	                                   &resizable);
	}
	else if (widget && GTK_IS_CLIST (widget->parent))
	{
		gtk_clist_get_header_index (GTK_CLIST(widget->parent),
		                            widget, &column_index, &columns);
	}
	
	if (area)
	{
		gdk_gc_set_clip_rectangle (clearlooks_style->shade_gc[0], area);
		gdk_gc_set_clip_rectangle (clearlooks_style->shade_gc[4], area);
		gdk_gc_set_clip_rectangle (style->bg_gc[state_type], area);			
		gdk_gc_set_clip_rectangle (clearlooks_style->shade_gc[5], area);
	}
	
	if (state_type != GTK_STATE_NORMAL)
		fill_width-=2;
	
	gdk_draw_rectangle (window, style->bg_gc[state_type], TRUE, x, y, fill_width, height-(height/3)+1);
	
	cl_draw_hgradient (window, style->bg_gc[state_type], style,
	                x, 1+y+height-(height/3), fill_width, height/3,
	                &style->bg[state_type], &clearlooks_style->inset_dark[state_type]);

	if (resizable || (column_index != columns-1))
	{
		gdk_draw_line (window, clearlooks_style->shade_gc[4], x+width-2, y+4, x+width-2, y+height-5); 
		gdk_draw_line (window, clearlooks_style->shade_gc[0], x+width-1, y+4, x+width-1, y+height-5); 
	}
	
	/* left light line */
	if (column_index == 0)
		gdk_draw_line (window, clearlooks_style->shade_gc[0], x, y+1, x, y+height-2);
		
	/* top light line */
	gdk_draw_line (window, clearlooks_style->shade_gc[0], x, y, x+width-1, y);
	
	/* bottom dark line */
	if (state_type == GTK_STATE_INSENSITIVE)
		bottom = clearlooks_style->shade_gc[3];
	
	
	gdk_draw_line (window, bottom, x, y+height-1, x+width-1, y+height-1);
	
	if (area)
	{
		gdk_gc_set_clip_rectangle (clearlooks_style->shade_gc[0], NULL);
		gdk_gc_set_clip_rectangle (clearlooks_style->shade_gc[4], NULL);
		gdk_gc_set_clip_rectangle (style->bg_gc[state_type], NULL);
		gdk_gc_set_clip_rectangle (clearlooks_style->shade_gc[5], NULL);
	}		
}

/******************************************************************************
 * PROGRESSBAR STYLE 1                                                        *
 ******************************************************************************/

static void cl_progressbar1_points_transform (GdkPoint *points, int npoints, 
                                             int offset, gboolean is_horizontal)
{
	int i;
	for ( i=0; i<npoints; i++) {
		if ( is_horizontal )
			points[i].x += offset;
		else
			points[i].y += offset;
	}
}

static
GdkPixmap* cl_progressbar1_create_tile (GdkDrawable *drawable,
                                        GtkWidget *widget,
                                        GtkStyle *style,
                                        gint height,
                                        gint offset)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	int width  = height;
	int xdir   = 1;
	int trans;

	int stripe_width   = height/2;
	int topright       = height + stripe_width;	
	int topright_div_2 = topright/2;

	double shift;	
	GdkPoint points[4];

	GtkProgressBarOrientation orientation = cl_get_progress_bar_orientation (widget);
	gboolean is_horizontal = (orientation == GTK_PROGRESS_LEFT_TO_RIGHT || orientation == GTK_PROGRESS_RIGHT_TO_LEFT) ? 1 : 0;
	
	GdkPixmap *tmp = gdk_pixmap_new (drawable, width, height, -1);

	GdkColor tmp_color;
	cl_shade (&clearlooks_style->spot2, &tmp_color, 0.90);
	
	if (is_horizontal)
		cl_draw_hgradient (tmp, style->black_gc, style, 0, 0, width, height,
	    	            &clearlooks_style->spot2, &tmp_color );
	else
		cl_draw_vgradient (tmp, style->black_gc, style, 0, 0, width, height,
	    	            &tmp_color, &clearlooks_style->spot2); /* TODO: swap for RTL */
	                
	if (orientation == GTK_PROGRESS_RIGHT_TO_LEFT || 
	    orientation == GTK_PROGRESS_BOTTOM_TO_TOP)
	{
		offset = -offset;
		xdir = -1;
	}
	
	if (cl_get_direction (widget) == GTK_TEXT_DIR_RTL)
		offset = -offset;
	
	if (is_horizontal)
	{
		points[0] = (GdkPoint){xdir*(topright - stripe_width - topright_div_2), 0};  /* topleft */
		points[1] = (GdkPoint){xdir*(topright - topright_div_2), 0};                 /* topright */
		points[2] = (GdkPoint){xdir*(stripe_width - topright_div_2), height};        /* bottomright */
		points[3] = (GdkPoint){xdir*(-topright_div_2), height};                      /* bottomleft */
	}
	else
	{
		points[0] = (GdkPoint){height, xdir*(topright - stripe_width - topright_div_2)};  /* topleft */
		points[1] = (GdkPoint){height, xdir*(topright - topright_div_2)};                 /* topright */
		points[2] = (GdkPoint){0, xdir*(stripe_width - topright_div_2)};        /* bottomright */
		points[3] = (GdkPoint){0, xdir*(-topright_div_2)};                      /* bottomleft */
	}
						 
	
	shift = (stripe_width*2)/(double)10;
	cl_progressbar1_points_transform (points, 4, (offset*shift), is_horizontal);
		
	trans = (width/2)-1-(stripe_width*2);
	cl_progressbar1_points_transform (points, 4, trans, is_horizontal);
	gdk_draw_polygon (tmp, clearlooks_style->spot2_gc, TRUE, points, 4);
	cl_progressbar1_points_transform (points, 4, -trans, is_horizontal);

	trans = width/2-1;
	cl_progressbar1_points_transform (points, 4, trans, is_horizontal);
	gdk_draw_polygon (tmp, clearlooks_style->spot2_gc, TRUE, points, 4);
	cl_progressbar1_points_transform (points, 4, -trans, is_horizontal);

	trans = (width/2)-1+(stripe_width*2);
	cl_progressbar1_points_transform (points, 4, trans, is_horizontal);
	gdk_draw_polygon (tmp, clearlooks_style->spot2_gc, TRUE, points, 4);
	
	return tmp;
}

/* could be improved, I think. */
static void
cl_progressbar1_fill (GdkDrawable *drawable, GtkWidget *widget,
                          GtkStyle *style, GdkGC *gc,
                          gint x, gint y,
                          gint width, gint height,
                          guint8 offset, GdkRectangle *area)
{
	GtkProgressBarOrientation orientation = cl_get_progress_bar_orientation (widget);
	gint size = (orientation == GTK_PROGRESS_LEFT_TO_RIGHT || orientation == GTK_PROGRESS_RIGHT_TO_LEFT) ? height : width;
	GdkPixmap *tile = cl_progressbar1_create_tile (drawable, widget, style, size, offset);

	gint nx = x,
	     ny = y,
	     nwidth = height,
	     nheight = width;
	
	gdk_gc_set_clip_rectangle (gc, area);
	
	switch (orientation)
	{
		case GTK_PROGRESS_LEFT_TO_RIGHT:
		{
			while (nx <= x + width )
			{
				if (nx + nwidth > x+width ) nwidth = (x+width) - nx;
				gdk_draw_drawable (drawable, gc, tile, 0, 0, nx, y, nwidth, height);
                                if (height <= 1)
                                    nx += 1;
                                else
				    nx += (height-1 + !(height % 2));
			}
			break;
		}
		case GTK_PROGRESS_RIGHT_TO_LEFT:
		{
			gint src_x = 0, dst_x;
			nx += width;
			while (nx >= x )
			{
				dst_x = nx - height;
				if (dst_x < x )
				{
					src_x = x - dst_x;
					dst_x = x;
				}
				gdk_draw_drawable (drawable, gc, tile, src_x, 0, dst_x, y, nwidth, height);
                                if (height <= 1)
                                    nx -= 1;
                                else
				    nx -= (height-1 + !(height % 2));
			}
			break;
		}
		case GTK_PROGRESS_TOP_TO_BOTTOM:
		{
			while (ny <= y + height )
			{
				if (ny + nheight > y+height ) nheight = (y+height) - ny;
				gdk_draw_drawable (drawable, gc, tile, 0, 0, x, ny, width, nheight);
                                if (width <= 1)
                                    ny += 1;
                                else
				    ny += (width-1 + !(width % 2));
			}
			break;
		}
		case GTK_PROGRESS_BOTTOM_TO_TOP:
		{
			gint src_y = 0, dst_y;
			ny += height;
			while (ny >= y )
			{
				dst_y = ny - width;
				if (dst_y < y )
				{
					src_y = y - dst_y;
					dst_y = y;
				}
				gdk_draw_drawable (drawable, gc, tile, 0, src_y, x, dst_y, width, width);
                                if (width <= 1)
                                    ny -= 1;
                                else
				    ny -= (width-1 + !(width % 2));
			}
			break;
		}
	}
	
	gdk_gc_set_clip_rectangle (gc, NULL);
	
	g_object_unref (tile);
}

void
cl_draw_progressbar1_trough(GdkDrawable *window, GtkWidget *widget,
                            GtkStyle *style, GtkStateType state_type,
                            gint x, gint y, gint width, gint height,
                            GdkRectangle *area)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	GdkPoint points[4] = { {x,y}, {x+width-1,y},
	                       {x,y+height-1}, {x+width-1,y+height-1} };
	CLRectangle r;
		
						   
	// Draw inset
	if (style->xthickness > 1 && style->ythickness > 1)
	{
		cl_draw_inset (style, window, widget, area, x, y, width, height,
		               CL_CORNER_NARROW, CL_CORNER_NARROW,
		               CL_CORNER_NARROW, CL_CORNER_NARROW);

		x++;
		y++;
		height-=2;
		width-=2;
	}	
						   
	gdk_draw_points (window, style->bg_gc[state_type], points, 4);

	cl_rectangle_init (&r, clearlooks_style->shade_gc[2],
	                       clearlooks_style->shade_gc[5],
	                       CL_CORNER_NARROW, CL_CORNER_NARROW,
	                       CL_CORNER_NARROW, CL_CORNER_NARROW);
		
	cl_rectangle_set_clip_rectangle (&r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	cl_rectangle_reset_clip_rectangle (&r);
}

void
cl_draw_progressbar1_fill(GdkDrawable *window, GtkWidget *widget,
                          GtkStyle *style, GtkStateType state_type,
                          gint x, gint y, gint width, gint height,
                          GdkRectangle *area, gint offset)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	GdkColor prev_foreground;
	gboolean activity_mode = GTK_PROGRESS (widget)->activity_mode;
	CLRectangle r;
	
	cl_progressbar1_fill (window, widget, style, style->black_gc,
						 x, y, width, height,
						 activity_mode ? 0 : offset,
						 area);
	
	cl_rectangle_init (&r, NULL,
	                       NULL,
	                       CL_CORNER_NONE, CL_CORNER_NONE,
	                       CL_CORNER_NONE, CL_CORNER_NONE);
	
	r.bordergc = clearlooks_style->spot3_gc;
	r.topleft = clearlooks_style->spot2_gc;
	
	prev_foreground = cl_gc_set_fg_color_shade (clearlooks_style->spot2_gc,
												style->colormap,
												&clearlooks_style->spot2,
												1.2);

	cl_rectangle_set_clip_rectangle (&r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	cl_draw_shadow (window, widget, style, x, y, width, height, &r);
	cl_rectangle_reset_clip_rectangle (&r);
	
	gdk_gc_set_foreground (clearlooks_style->spot2_gc, &prev_foreground);
}

/******************************************************************************
 * PROGRESSBAR STYLE 2                                                        *
 ******************************************************************************/

void
cl_draw_progressbar2_trough(GdkDrawable *window, GtkWidget *widget,
                            GtkStyle *style, GtkStateType state_type,
                            gint x, gint y, gint width, gint height,
                            GdkRectangle *area, gboolean horizontal)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	CLRectangle r;
	GdkColor c, d;
	cl_shade(&style->bg[0], &c, 0.95);
	cl_rectangle_init (&r, clearlooks_style->shade_gc[2],
	                       clearlooks_style->border_gc[CL_BORDER_UPPER],
	                       CL_CORNER_NARROW, CL_CORNER_NARROW,
	                       CL_CORNER_NARROW, CL_CORNER_NARROW);

	r.gradient_type = horizontal ? CL_GRADIENT_VERTICAL : CL_GRADIENT_HORIZONTAL;
	
	cl_rectangle_set_gradient (&r.fill_gradient, &clearlooks_style->cl_shade[2],
	                                             &clearlooks_style->cl_shade[1]);
	
	// Draw inset
	if (style->xthickness > 1 && style->ythickness > 1)
	{
		cl_draw_inset (style, window, widget, area, x, y, width, height,
		               CL_CORNER_NARROW, CL_CORNER_NARROW,
		               CL_CORNER_NARROW, CL_CORNER_NARROW);

		x++;
		y++;
		height-=2;
		width-=2;
	}	
	
	cl_shade(&style->bg[0], &d, 0.80);
	
	// Draw the trough itself
	cl_rectangle_set_clip_rectangle (&r, area);
	cl_draw_rectangle (window, widget, style, x, y, width, height, &r);
	cl_rectangle_set_gradient (&r.fill_gradient, &clearlooks_style->cl_shade[3],
	                                             &clearlooks_style->cl_shade[2]);
	cl_draw_fill (window, widget, style, x, y, width, 4, &r);
	cl_rectangle_reset_clip_rectangle (&r);
}

static void
cl_progressbar2_set_four_points(GtkWidget *widget, GdkPoint *points, int size)
{
	GtkProgressBarOrientation orientation = cl_get_progress_bar_orientation (widget);

	switch (orientation)
	{
		case GTK_PROGRESS_LEFT_TO_RIGHT:
			points[0].x = size;          points[0].y = 0;
			points[1].x = size*2;        points[1].y = 0;
			points[2].x = size;          points[2].y = size;
			points[3].x = 0;             points[3].y = size;
			break;
		case GTK_PROGRESS_RIGHT_TO_LEFT:
			points[0].x = 0;             points[0].y = 0;
			points[1].x = size;          points[1].y = 0;
			points[2].x = size*2;        points[2].y = size;
			points[3].x = size;          points[3].y = size;
			break;
		case GTK_PROGRESS_TOP_TO_BOTTOM:
			points[0].x = 0;             points[0].y = 0;
			points[1].x = size;            points[1].y = size;
			points[2].x = size;            points[2].y = size*2;
			points[3].x = 0;             points[3].y = size;
			break;
		case GTK_PROGRESS_BOTTOM_TO_TOP:
			points[0].x = size;             points[0].y = 0;
			points[1].x = size;            points[1].y = size;
			points[2].x = 0;            points[2].y = size*2;
			points[3].x = 0;             points[3].y = size;
			break;
		default: 
			points[0].x = 0;               points[0].y = 0;
			points[1].x = 0;               points[1].y = 0;
			points[2].x = 0;               points[2].y = 0;
			points[3].x = 0;               points[3].y = 0;
			break;
	}
}

static GdkPixmap*
cl_progressbar2_create_tile (GtkWidget *widget, GtkStyle *style, gint size, gboolean horizontal)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	int width, height;
	GdkPixmap *tile;
	GdkColor l1, l2, l3;
	GdkColor d1, d2, d3;
	GdkPoint points[4];
	GdkRegion *region;
	int height1, height2; /* sizes of both gradients */
	
	cl_progressbar2_set_four_points(widget, points, size);

	region = gdk_region_polygon (points, 4, GDK_WINDING_RULE);

	if (horizontal)
	{
		width = size*2;
		height = size;
	}
	else
	{
		width = size;
		height = size*2;
	}
	
	height1 = (float)size*0.6;
	height2 = size - height1;

	tile = gdk_pixmap_new (widget->window, width, height, -1);

	l3 = clearlooks_style->spot2;
	cl_shade (&l3, &l1, 1.05);
	cl_shade (&l3, &l2, 1.2);
	
	cl_shade (&l3, &d3, 0.92);
	cl_shade (&d3, &d1, 1.05);
	cl_shade (&d3, &d2, 1.2);	

	if (horizontal)
	{
		cl_draw_hgradient (tile, style->bg_gc[0], style, 0, 0,         width, 3,       &l1, &l2);
		cl_draw_hgradient (tile, style->bg_gc[0], style, 0, 3,         width, height1, &l2, &l3);
		cl_draw_hgradient (tile, style->bg_gc[0], style, 0, height1+3, width, height2, &l3, &l2);
	}
	else
	{
		cl_draw_vgradient (tile, style->bg_gc[0], style, 0,         0, 3,       height, &l1, &l2);
		cl_draw_vgradient (tile, style->bg_gc[0], style, 3,         0, height1, height, &l2, &l3);
		cl_draw_vgradient (tile, style->bg_gc[0], style, height1+3, 0, height2, height, &l3, &l2);
	}
	
	/* And the angled stripes */
	
	gdk_gc_set_clip_region (clearlooks_style->spot3_gc, region);

	if (horizontal)
	{	
		cl_draw_hgradient (tile, clearlooks_style->spot3_gc, style, 0, 0,         width, 3,       &d1, &d2);
		cl_draw_hgradient (tile, clearlooks_style->spot3_gc, style, 0, 3,         width, height1, &d2, &d3);
		cl_draw_hgradient (tile, clearlooks_style->spot3_gc, style, 0, height1+3, width, height2, &d3, &d2);
	}	
	else
	{
		cl_draw_vgradient (tile, clearlooks_style->spot3_gc, style, 0,         0, 3,       height, &d1, &d2);
		cl_draw_vgradient (tile, clearlooks_style->spot3_gc, style, 3,         0, height1, height, &d2, &d3);
		cl_draw_vgradient (tile, clearlooks_style->spot3_gc, style, height1+3, 0, height2, height, &d3, &d2);
	}

	gdk_gc_set_clip_region (clearlooks_style->spot3_gc, NULL);
	
	gdk_region_destroy (region);
	
	return tile;
}

static void
cl_progressbar2_draw_fill(GdkDrawable *window, GdkGC *gc, GdkPixmap *tile,
                          gint x, gint y, gint width, gint height,
                          GtkOrientation orientation, gint offset)
{
	if (orientation == GTK_PROGRESS_LEFT_TO_RIGHT)
	{
		int height2 = height*2;
		int noffset = (((float)height2/10)*offset);
		int nx      = x+noffset;

		if (nx != x)
			gdk_draw_drawable (window, gc, tile, 0, 0, x-(height2-noffset), y, height2, height);
		
		while (nx <= x+width)
		{
			gdk_draw_drawable (window, gc, tile, 0, 0, nx, y, height2, height);
			nx += height2;
		}
	}
	else if (orientation == GTK_PROGRESS_RIGHT_TO_LEFT)
	{
		int height2 = height*2;
		int noffset = (((float)height2/10)*offset);
		int nx = x+width-1-noffset;
		
		while (nx+height2 >= x)
		{
			gdk_draw_drawable (window, gc, tile, 0, 0, nx, y, height2, height);
			nx -= height2;
		}
	}
	else if (orientation == GTK_PROGRESS_TOP_TO_BOTTOM)
	{
		int width2 = width*2;
		int noffset = (((float)width2/10)*offset);
		int ny = y+noffset;

		if (ny != y)
			gdk_draw_drawable (window, gc, tile, 0, 0, x, y-(width2-noffset), width, width2);

		while (ny <= y+height)
		{
			gdk_draw_drawable (window, gc, tile, 0, 0, x, ny, width, width2);
			ny += width2;
		}			
	}
	else if (orientation == GTK_PROGRESS_BOTTOM_TO_TOP)
	{
		int width2 = width*2;
		int noffset = (((float)width2/10)*offset);
		int ny = y+height-1-noffset;

		while (ny+width2 >= y)
		{
			gdk_draw_drawable (window, gc, tile, 0, 0, x, ny, width, width2);
			ny -= width2;
		}			
	}
}

void
cl_draw_progressbar2_fill(GdkDrawable *window, GtkWidget *widget,
                          GtkStyle *style, GtkStateType state_type,
                          gint x, gint y, gint width, gint height,
                          GdkRectangle *area, gint offset)
{
	ClearlooksStyle           *clearlooks_style = CLEARLOOKS_STYLE (style);
	GdkPixmap                 *tile;
	GdkPoint                  border_line[2];
	int                       shadow_pos;
	GtkProgressBarOrientation orientation;
	gboolean                  is_horizontal;
	GdkRectangle              fill_clip;
	
	if (width < 1 || height < 1)
		return;

	orientation = cl_get_progress_bar_orientation (widget);
	
	is_horizontal = orientation < 2;

	tile = cl_progressbar2_create_tile(widget, style,
	                                   is_horizontal ? height : width,
	                                   is_horizontal);
	
	fill_clip.x = x;
	fill_clip.y = y;
	fill_clip.width = width;
	fill_clip.height = height;
	
	switch (orientation)
	{
		case GTK_PROGRESS_LEFT_TO_RIGHT:
			border_line[0].x = x+width-1;
			border_line[0].y = y;
			border_line[1].x = x+width-1;
			border_line[1].y = y+height-1;
			shadow_pos = 1;
			break;
		case GTK_PROGRESS_RIGHT_TO_LEFT:
			border_line[0].x = x;
			border_line[0].y = y;
			border_line[1].x = x;
			border_line[1].y = y+height-1;
			shadow_pos = -2;
			break;
		case GTK_PROGRESS_TOP_TO_BOTTOM:
			border_line[0].x = x;
			border_line[0].y = y+height-1;
			border_line[1].x = x+width-1;
			border_line[1].y = y+height-1;
			shadow_pos = 1;
			break;
		case GTK_PROGRESS_BOTTOM_TO_TOP:
			border_line[0].x = x;
			border_line[0].y = y;
			border_line[1].x = x+width-1;
			border_line[1].y = y;
			shadow_pos = -2;
			break;
	}
		
	gdk_gc_set_clip_rectangle (clearlooks_style->spot2_gc, &fill_clip);
	cl_progressbar2_draw_fill (window, clearlooks_style->spot2_gc, tile,
	                           x, y, width, height, orientation, offset);
	gdk_gc_set_clip_rectangle (clearlooks_style->spot2_gc, NULL);
	
	/* Don't draw shadow & border line when completely filled */
	if (gtk_progress_bar_get_fraction(GTK_PROGRESS_BAR(widget)) != 1.0)
	{
		gdk_draw_line (window, clearlooks_style->spot2_gc,
		               border_line[0].x, border_line[0].y,
		               border_line[1].x, border_line[1].y);
		
		if (is_horizontal)
			cl_draw_vgradient (window, style->bg_gc[state_type], style,
			                border_line[0].x+shadow_pos, y, 2, height,
			                &clearlooks_style->cl_shade[3],
			                &clearlooks_style->cl_shade[2]);
		else
			cl_draw_hgradient (window, style->bg_gc[state_type], style,
			                x, border_line[0].y+shadow_pos, width, 2,
			                &clearlooks_style->cl_shade[3],
			                &clearlooks_style->cl_shade[2]);
	}
	
	g_object_unref (tile);
}

/******************************************************************************
 * PROGRESSBAR STYLE 2 (flat)                                                 *
 ******************************************************************************/

void
cl_draw_progressbar3_fill(GdkDrawable *window, GtkWidget *widget,
                          GtkStyle *style, GtkStateType state_type,
                          gint x, gint y, gint width, gint height,
                          GdkRectangle *area)
{
	ClearlooksStyle *clearlooks_style = CLEARLOOKS_STYLE (style);
	
	gdk_draw_rectangle (window, clearlooks_style->spot2_gc, TRUE,
	                    x, y, width, height);
}
