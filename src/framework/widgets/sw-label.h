/* sw-label.h
 *
 * SwLabel: static text display, one of the day-zero widgets
 * (docs/proposal/0003-day-zero-widget-set.md section 4). No icon
 * support yet; that is a separate asset-handling concern deferred past
 * day zero.
 */

#ifndef SW_LABEL_H
#define SW_LABEL_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

#include "sw-widget.h"

G_BEGIN_DECLS

#define SW_TYPE_LABEL (sw_label_get_type ())

G_DECLARE_FINAL_TYPE (SwLabel, sw_label, SW, LABEL, SwWidget)

/**
 * sw_label_new:
 * @text: (nullable): the label's initial text, or %NULL for an empty
 *   label
 *
 * Creates a new #SwLabel.
 *
 * Ownership: transfer-full (floating, like any `GtkWidget`).
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer full): a new #SwLabel.
 */
GtkWidget *sw_label_new (const char *text);

/**
 * sw_label_set_text:
 * @self: a #SwLabel
 * @text: (nullable): the new text, or %NULL for an empty label
 *
 * Sets the label's text. Per docs/foundational/design-doc.md section 5,
 * whatever typography role and color this renders with must still meet
 * that section's contrast requirement at the label's actual rendered
 * size; that is the default theme's responsibility, not this
 * function's.
 *
 * Ownership: transfer-none on @text; @self copies it.
 *
 * Thread-affinity: main-thread only.
 */
void sw_label_set_text (SwLabel    *self,
                         const char *text);

/**
 * sw_label_get_text:
 * @self: a #SwLabel
 *
 * Ownership: transfer-none. The returned string is owned by @self and
 * is only valid until the next call that changes the label's text.
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer none): the label's current text.
 */
const char *sw_label_get_text (SwLabel *self);

G_END_DECLS

#endif /* SW_LABEL_H */
