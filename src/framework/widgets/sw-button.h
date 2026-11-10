/* sw-button.h
 *
 * SwButton: a single clickable action control, one of the day-zero
 * widgets (docs/proposal/0003-day-zero-widget-set.md section 4). Day
 * zero ships one visual variant; filled/outlined/text variant taxonomy
 * is explicitly deferred past this proposal. What day zero's SwButton
 * must prove is the interaction contract: pointer press/release/cancel,
 * keyboard activation, a focus-visible state, and a correct accessible
 * role/name.
 */

#ifndef SW_BUTTON_H
#define SW_BUTTON_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

#include "sw-widget.h"

G_BEGIN_DECLS

#define SW_TYPE_BUTTON (sw_button_get_type ())

G_DECLARE_FINAL_TYPE (SwButton, sw_button, SW, BUTTON, SwWidget)

/**
 * sw_button_new:
 * @label: (nullable): the button's initial label text, or %NULL
 *
 * Creates a new #SwButton.
 *
 * Ownership: transfer-full (floating, like any `GtkWidget`).
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer full): a new #SwButton.
 */
GtkWidget *sw_button_new (const char *label);

/**
 * sw_button_set_label:
 * @self: a #SwButton
 * @label: (nullable): the new label text, or %NULL for an empty button
 *
 * Ownership: transfer-none on @label; @self copies it.
 *
 * Thread-affinity: main-thread only.
 */
void sw_button_set_label (SwButton   *self,
                           const char *label);

/**
 * sw_button_get_label:
 * @self: a #SwButton
 *
 * Ownership: transfer-none. Valid only until the next call that
 * changes the button's label.
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer none): the button's current label text.
 */
const char *sw_button_get_label (SwButton *self);

/* SwButton::clicked, emitted on pointer release-inside or keyboard
 * activation (Space/Enter while focused), is declared via
 * g_signal_new() in sw-button.c, following the same pattern as
 * GtkButton::clicked; there is no action-specific data to pass, so no
 * accompanying typedef is needed here. */

G_END_DECLS

#endif /* SW_BUTTON_H */
