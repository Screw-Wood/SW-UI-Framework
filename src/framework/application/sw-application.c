/* sw-application.c */

#define SW_COMPILATION
#include "sw-application.h"
#include "core/sw-init.h"

struct _SwApplication
{
  GtkApplication parent_instance;
};

G_DEFINE_TYPE (SwApplication, sw_application, GTK_TYPE_APPLICATION)

static void
sw_application_class_init (SwApplicationClass *klass)
{
  (void) klass;

  /* No overrides yet: startup/activate/shutdown mechanics are left to
   * GtkApplication/GApplication per
   * docs/proposal/0001-dependency-ownership-policy.md section 3. This
   * type exists as SW's own named entry point so application code
   * constructs an SwApplication, not a bare GtkApplication, and so
   * SW-owned application-level behavior has a documented place to be
   * added later without an API break. */
}

static void
sw_application_init (SwApplication *self)
{
  (void) self;
}

SwApplication *
sw_application_new (const char       *application_id,
                     GApplicationFlags flags)
{
  g_return_val_if_fail (sw_is_initialized (), NULL);
  g_return_val_if_fail (application_id == NULL ||
                         g_application_id_is_valid (application_id), NULL);

  return g_object_new (SW_TYPE_APPLICATION,
                        "application-id", application_id,
                        "flags", flags,
                        NULL);
}
