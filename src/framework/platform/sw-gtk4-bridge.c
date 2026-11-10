/* sw-gtk4-bridge.c */

#include "sw-gtk4-bridge.h"

#include <gtk/gtk.h>

gboolean
sw_gtk4_bridge_check_minimum_version (guint major,
                                       guint minor,
                                       guint micro)
{
  /* gtk_check_version() returns NULL when the runtime version satisfies
   * the request and a (non-owned, static) explanatory string otherwise. */
  return gtk_check_version (major, minor, micro) == NULL;
}
