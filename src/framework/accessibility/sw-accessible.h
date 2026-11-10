/* sw-accessible.h
 *
 * A small shared helper for SW's accessibility contract
 * (docs/foundational/SYSTEM_DESIGN.md section 11, and the baseline
 * requirements in docs/foundational/Architecture.md section 6). Every
 * #SwWidget subclass is expected to expose a real #GtkAccessibleRole
 * (see docs/proposal/0003-day-zero-widget-set.md section 4); this
 * module exists so that expectation can be checked programmatically
 * (by SW's own tests, or an application's) rather than only by
 * convention.
 *
 * This does not replace a real screen-reader review, which
 * docs/proposal/0003-day-zero-widget-set.md section 7 and
 * docs/foundational/SYSTEM_DESIGN.md section 15 both require before a
 * widget's accessibility contract is considered proven; it only checks
 * the structural precondition for that review (a role is actually set).
 */

#ifndef SW_ACCESSIBLE_H
#define SW_ACCESSIBLE_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

/**
 * sw_accessible_has_role:
 * @accessible: (transfer none): a #GtkAccessible
 *
 * Checks that @accessible has been given a meaningful
 * #GtkAccessibleRole, i.e. anything other than %GTK_ACCESSIBLE_ROLE_NONE
 * or %GTK_ACCESSIBLE_ROLE_GENERIC. Intended for use from SW's own
 * widget/accessibility tests (docs/foundational/SYSTEM_DESIGN.md
 * section 15), not as a runtime check in application code.
 *
 * Ownership: transfer-none on @accessible.
 *
 * Thread-affinity: main-thread only, like all GTK accessible API.
 *
 * Returns: %TRUE if @accessible exposes a specific accessible role.
 */
gboolean sw_accessible_has_role (GtkAccessible *accessible);

G_END_DECLS

#endif /* SW_ACCESSIBLE_H */
