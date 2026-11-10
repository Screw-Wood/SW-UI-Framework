/* sw-application.h
 *
 * SwApplication is the thin application-level layer on top of
 * SwWindow's lifecycle (docs/proposal/0002-lifecycle-ownership-model.md
 * section 3): its own startup/shutdown states map directly onto
 * `GApplication::startup`/`::shutdown`, which GTK4 already guarantees
 * fire once, only on the primary instance, so there is nothing
 * SW-specific to enforce at this layer beyond construction.
 */

#ifndef SW_APPLICATION_H
#define SW_APPLICATION_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SW_TYPE_APPLICATION (sw_application_get_type ())

G_DECLARE_FINAL_TYPE (SwApplication, sw_application, SW, APPLICATION, GtkApplication)

/**
 * sw_application_new:
 * @application_id: (nullable): a valid D-Bus application ID, as
 *   required by `GApplication`; see g_application_id_is_valid().
 * @flags: application flags, passed through to `GApplication`.
 *
 * Creates a new #SwApplication. sw_init() must have already been
 * called successfully.
 *
 * Ownership: transfer-full.
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer full): a new #SwApplication.
 */
SwApplication *sw_application_new (const char      *application_id,
                                    GApplicationFlags flags);

G_END_DECLS

#endif /* SW_APPLICATION_H */
