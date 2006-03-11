#ifndef CRUX_RC_PARSER_H
#define CRUX_RC_PARSER_H

#include <gtk/gtk.h>
#include "crux-common.h"

guint
crux_parse_rc_style  (GtkRcStyle   *rc_style,
		      GtkSettings  *settings,
		      GScanner     *scanner) G_GNUC_INTERNAL;

void
crux_theme_data_ref (eazel_theme_data *theme_data) G_GNUC_INTERNAL;

void
crux_theme_data_unref (eazel_theme_data *theme_data) G_GNUC_INTERNAL;

void crux_rc_parser_init_gradients (void) G_GNUC_INTERNAL;
#endif
