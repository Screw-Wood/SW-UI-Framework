/* sw-box.h
 *
 * SwBox is the single day-zero layout primitive
 * (docs/proposal/0003-day-zero-widget-set.md section 4): horizontal or
 * vertical stacking with spacing, aligned to the
 * docs/foundational/design-doc.md section 2 8dp grid. No grid, flow, or
 * scrolling container yet; those are later layout work per
 * docs/foundational/SYSTEM_DESIGN.md section 9.
 */

#ifndef SW_BOX_H
#define SW_BOX_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

#include "sw-widget.h"

G_BEGIN_DECLS

#define SW_TYPE_BOX (sw_box_get_type ())

G_DECLARE_FINAL_TYPE (SwBox, sw_box, SW, BOX, SwWidget)

/**
 * sw_box_new:
 * @orientation: whether the box stacks children horizontally or
 *   vertically
 * @spacing: spacing between children, in dp. Callers should prefer a
 *   multiple of %SW_METRICS_GRID_BASELINE_DP where practical, per
 *   docs/foundational/design-doc.md section 2; this is a convention,
 *   not an enforced constraint.
 *
 * Creates a new #SwBox.
 *
 * Ownership: transfer-full (floating, like any `GtkWidget`).
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer full): a new #SwBox.
 */
GtkWidget *sw_box_new (GtkOrientation orientation,
                        int            spacing);

/**
 * sw_box_append:
 * @self: a #SwBox
 * @child: (transfer none): the widget to append
 *
 * Appends @child to the end of @self.
 *
 * Ownership: transfer-none on @child; @self takes the floating
 * reference, as GTK containers conventionally do.
 *
 * Thread-affinity: main-thread only.
 */
void sw_box_append (SwBox     *self,
                     GtkWidget *child);

/**
 * sw_box_remove:
 * @self: a #SwBox
 * @child: (transfer none): a widget previously appended to @self
 *
 * Removes @child from @self.
 *
 * Ownership: transfer-none on @child.
 *
 * Thread-affinity: main-thread only.
 */
void sw_box_remove (SwBox     *self,
                     GtkWidget *child);

G_END_DECLS

#endif /* SW_BOX_H */
