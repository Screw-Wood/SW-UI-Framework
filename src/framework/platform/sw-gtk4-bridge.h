/* sw-gtk4-bridge.h
 *
 * Internal-only helpers for SW's GTK4 integration layer
 * (docs/foundational/SYSTEM_DESIGN.md section 5). This header is not
 * installed and is not part of SW's public API: it exists so the few
 * places that need to reason about the underlying GTK4 version or
 * integration detail have one shared, owned place to do it, instead of
 * scattering direct gtk_get_*_version() calls through the codebase.
 */

#ifndef SW_GTK4_BRIDGE_H
#define SW_GTK4_BRIDGE_H

#include <glib.h>

G_BEGIN_DECLS

/* Internal: checks that the GTK4 actually linked at runtime is at least
 * @major.@minor.@micro, the way GTK's own gtk_check_version() works.
 * Used by sw_init() to fail loudly and early rather than hitting an
 * unrelated crash deep inside a widget later. */
gboolean sw_gtk4_bridge_check_minimum_version (guint major,
                                                guint minor,
                                                guint micro);

G_END_DECLS

#endif /* SW_GTK4_BRIDGE_H */
