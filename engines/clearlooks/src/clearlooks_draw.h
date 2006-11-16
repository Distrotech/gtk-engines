#ifndef CLEARLOOKS_DRAW_H
#define CLEARLOOKS_DRAW_H

#include "clearlooks_types.h"

#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include <cairo.h>

GE_INTERNAL void clearlooks_draw_button           (cairo_t *cr,
                                       const ClearlooksColors      *colors,
                                       const WidgetParameters      *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_scale_trough     (cairo_t *cr,
                                       const ClearlooksColors      *colors,
                                       const WidgetParameters      *widget,
                                       const SliderParameters      *slider,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_progressbar_trough (cairo_t *cr,
                                       const ClearlooksColors      *colors,
                                       const WidgetParameters      *widget,
                                       int x, int y, int width, int height);


GE_INTERNAL void clearlooks_draw_progressbar_fill (cairo_t *cr,
                                       const ClearlooksColors      *colors,
                                       const WidgetParameters      *widget,
                                       const ProgressBarParameters *progressbar,
                                       int x, int y, int width, int height, gint offset);

GE_INTERNAL void clearlooks_draw_slider_button    (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       const SliderParameters     *slider,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_entry            (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_spinbutton       (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_spinbutton_down  (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_optionmenu       (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       const OptionMenuParameters *optionmenu,
                                       int x, int y, int width, int height);
								   
GE_INTERNAL void clearlooks_draw_inset            (cairo_t *cr,
                                       int width, int height,
                                       double radius, uint8 corners);

GE_INTERNAL void clearlooks_draw_menubar          (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       const MenuBarParameters    *menubar,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_tab              (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       const TabParameters        *tab,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_frame            (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       const FrameParameters      *frame,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_separator        (cairo_t *cr,
                                       const ClearlooksColors     *colors,
                                       const WidgetParameters     *widget,
                                       const SeparatorParameters  *separator,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_list_view_header (cairo_t *cr,
                                       const ClearlooksColors         *colors,
                                       const WidgetParameters         *widget,
                                       const ListViewHeaderParameters *header,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_toolbar          (cairo_t *cr,
                                       const ClearlooksColors         *colors,
                                       const WidgetParameters         *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_menuitem         (cairo_t *cr,
                                       const ClearlooksColors         *colors,
                                       const WidgetParameters         *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_scrollbar_stepper (cairo_t *cr,
                                        const ClearlooksColors           *colors,
                                        const WidgetParameters           *widget,
                                        const ScrollBarParameters        *scrollbar,
                                        const ScrollBarStepperParameters *stepper,
                                        int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_scrollbar_slider (cairo_t *cr,
                                       const ClearlooksColors          *colors,
                                       const WidgetParameters          *widget,
                                       const ScrollBarParameters       *scrollbar,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_scrollbar_trough (cairo_t *cr,
                                       const ClearlooksColors          *colors,
                                       const WidgetParameters          *widget,
                                       const ScrollBarParameters       *scrollbar,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_statusbar        (cairo_t *cr,
                                       const ClearlooksColors          *colors,
                                       const WidgetParameters          *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_menu_frame       (cairo_t *cr,
                                       const ClearlooksColors          *colors,
                                       const WidgetParameters          *widget,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_handle           (cairo_t *cr,
                                       const ClearlooksColors          *colors,
                                       const WidgetParameters          *widget,
                                       const HandleParameters          *handle,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_arrow            (cairo_t *cr,
                                       const ClearlooksColors          *colors,
                                       const WidgetParameters          *widget,
                                       const ArrowParameters           *arrow,
                                       int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_resize_grip      (cairo_t *cr,
                                       const ClearlooksColors          *colors,
                                       const WidgetParameters          *widget,
                                       const ResizeGripParameters      *grip,
                                       int x, int y, int width, int height);
                                       
GE_INTERNAL void clearlooks_draw_checkbox (cairo_t *cr,
                                           const ClearlooksColors  *colors,
                                           const WidgetParameters   *widget,
                                           const CheckboxParameters *checkbox,
                                           int x, int y, int width, int height);

GE_INTERNAL void clearlooks_draw_radiobutton (cairo_t *cr,
                                              const ClearlooksColors *colors,
                                              const WidgetParameters  *widget,
                                              const CheckboxParameters *checkbox,
                                              int x, int y, int width, int height);

/* Function Pointers */
typedef void (*menubar_draw_proto) (cairo_t *cr,
                                    const ClearlooksColors *colors,
                                    const WidgetParameters *params,
                                    const MenuBarParameters *menubar,
                                    int x, int y, int width, int height);

#endif /* CLEARLOOKS_DRAW_H */
