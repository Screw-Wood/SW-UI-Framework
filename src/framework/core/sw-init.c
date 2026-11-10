/* sw-init.c */

#define SW_COMPILATION
#include "sw-init.h"
#include "platform/sw-gtk4-bridge.h"

#include <gtk/gtk.h>

static gboolean sw_initialized = FALSE;

gboolean
sw_init (int    *argc,
         char ***argv)
{
  if (sw_initialized)
    return TRUE;

  /* Platform mechanics (process/display bring-up) are outsourced to
   * GTK4 per docs/proposal/0001-dependency-ownership-policy.md section 3;
   * sw_gtk4_bridge_check_minimum_version() guards the one SW-owned
   * precondition on top of that: that the GTK4 SW was built against is
   * still what is actually linked at runtime. */
  if (!sw_gtk4_bridge_check_minimum_version (GTK_MAJOR_VERSION,
                                              GTK_MINOR_VERSION,
                                              0))
    {
      g_critical ("SW requires GTK4 >= %d.%d.0 at runtime",
                  GTK_MAJOR_VERSION, GTK_MINOR_VERSION);
      return FALSE;
    }

  gtk_init ();

  if (argc != NULL && argv != NULL)
    {
      /* gtk_init() in GTK4 no longer consumes argc/argv itself; accepted
       * here purely so SW's own entry point can gain argument handling
       * later (per SYSTEM_DESIGN.md section 18) without an API break. */
    }

  sw_initialized = TRUE;
  return TRUE;
}

gboolean
sw_is_initialized (void)
{
  return sw_initialized;
}
