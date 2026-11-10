/* sw-widget-private.h
 *
 * Not installed, not part of SW's public API. Concrete #SwWidget
 * subclasses (SwButton, SwEntry, ...) need to flip interaction-state
 * flags their own input handling detects (e.g. %SW_WIDGET_STATE_PRESSED
 * on a click gesture); this is the one function that is allowed to do
 * that, so state changes always go through sw-widget.c's single
 * "state-changed" emission path rather than each subclass reinventing
 * it.
 */

#ifndef SW_WIDGET_PRIVATE_H
#define SW_WIDGET_PRIVATE_H

#include "sw-widget.h"

G_BEGIN_DECLS

void _sw_widget_set_state (SwWidget      *self,
                            SwWidgetState  state,
                            gboolean       set);

G_END_DECLS

#endif /* SW_WIDGET_PRIVATE_H */
