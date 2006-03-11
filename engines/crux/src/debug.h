#ifndef DEBUG_H
#define DEBUG_H

#include <gtk/gtk.h>

void
stop_in_debugger (void) G_GNUC_INTERNAL;
void make_warnings_and_criticals_stop_in_debugger (void) G_GNUC_INTERNAL;
#endif
