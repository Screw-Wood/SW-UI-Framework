/* sw-entry.h
 *
 * SwEntry: single-line text input, one of the day-zero widgets
 * (docs/proposal/0003-day-zero-widget-set.md section 4). This is the
 * widget that exercises SW's lifecycle-observer interface
 * (docs/proposal/0002-lifecycle-ownership-model.md section 4) in
 * practice: it commits its value when its owning #SwWindow becomes
 * %SW_WINDOW_STATE_UNFOCUSED, the concrete example that proposal uses
 * to justify the observer pattern. Multi-line/textarea support is out
 * of scope here.
 */

#ifndef SW_ENTRY_H
#define SW_ENTRY_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

#include "sw-widget.h"

G_BEGIN_DECLS

#define SW_TYPE_ENTRY (sw_entry_get_type ())

G_DECLARE_FINAL_TYPE (SwEntry, sw_entry, SW, ENTRY, SwWidget)

/**
 * sw_entry_new:
 *
 * Creates a new, empty #SwEntry.
 *
 * Ownership: transfer-full (floating, like any `GtkWidget`).
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer full): a new #SwEntry.
 */
GtkWidget *sw_entry_new (void);

/**
 * sw_entry_set_text:
 * @self: a #SwEntry
 * @text: (nullable): the new text, or %NULL to clear it
 *
 * Sets the entry's current (uncommitted) text, without emitting
 * #SwEntry::committed. Use this for initializing or programmatically
 * resetting the field, not for reacting to the user's own typing.
 *
 * Ownership: transfer-none on @text; @self copies it.
 *
 * Thread-affinity: main-thread only.
 */
void sw_entry_set_text (SwEntry    *self,
                         const char *text);

/**
 * sw_entry_get_text:
 * @self: a #SwEntry
 *
 * Ownership: transfer-none. Valid only until the next call that
 * changes the entry's text.
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer none): the entry's current text.
 */
const char *sw_entry_get_text (SwEntry *self);

/* SwEntry::committed, emitted with the entry's current text when its
 * owning SwWindow transitions to SW_WINDOW_STATE_UNFOCUSED (the
 * lifecycle-observer integration named in
 * docs/proposal/0002-lifecycle-ownership-model.md section 4), is
 * declared via g_signal_new() in sw-entry.c. */

G_END_DECLS

#endif /* SW_ENTRY_H */
