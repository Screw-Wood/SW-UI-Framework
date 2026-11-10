/* sw-widget.h
 *
 * SwWidget is the shared base every concrete SW widget is built on,
 * per docs/proposal/0003-day-zero-widget-set.md section 4: it is what
 * the lifecycle-observer interface, the accessibility contract, and
 * SYSTEM_DESIGN.md section 10's interaction-state model attach to once,
 * rather than being reimplemented per concrete widget. It is not
 * directly instantiable.
 */

#ifndef SW_WIDGET_H
#define SW_WIDGET_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define SW_TYPE_WIDGET (sw_widget_get_type ())

G_DECLARE_DERIVABLE_TYPE (SwWidget, sw_widget, SW, WIDGET, GtkWidget)

/**
 * SwWidgetState:
 * @SW_WIDGET_STATE_NORMAL: the resting state, no other flag set.
 * @SW_WIDGET_STATE_HOVER: the pointer is over the widget.
 * @SW_WIDGET_STATE_PRESSED: the widget is being pressed (pointer down
 *   or equivalent).
 * @SW_WIDGET_STATE_FOCUSED: the widget has keyboard focus.
 * @SW_WIDGET_STATE_SELECTED: the widget is in a selected state, for
 *   widgets where selection is meaningful.
 * @SW_WIDGET_STATE_DISABLED: the widget does not accept input.
 *
 * The interaction states from docs/foundational/SYSTEM_DESIGN.md
 * section 10. This is a flags enum: a widget can be, for example, both
 * %SW_WIDGET_STATE_HOVER and %SW_WIDGET_STATE_FOCUSED at once.
 * docs/proposal/0003-day-zero-widget-set.md section 4 requires every
 * #SwWidget to expose at least normal, focused, and disabled.
 */
typedef enum /*< flags >*/
{
  SW_WIDGET_STATE_NORMAL   = 0,
  SW_WIDGET_STATE_HOVER    = 1 << 0,
  SW_WIDGET_STATE_PRESSED  = 1 << 1,
  SW_WIDGET_STATE_FOCUSED  = 1 << 2,
  SW_WIDGET_STATE_SELECTED = 1 << 3,
  SW_WIDGET_STATE_DISABLED = 1 << 4,
} SwWidgetState;

struct _SwWidgetClass
{
  GtkWidgetClass parent_class;

  /**
   * SwWidgetClass::state_changed:
   * @self: the #SwWidget
   * @state: the new #SwWidgetState flags
   *
   * Class handler for #SwWidget::state-changed. The default
   * implementation updates the `state-*` CSS node classes used by SW's
   * default theme; subclasses overriding this must chain up.
   */
  void (* state_changed) (SwWidget      *self,
                           SwWidgetState  state);

  /* Reserved for future virtual methods; keeps ABI room per
   * Architecture.md section 2's parallel-installability policy. */
  gpointer _sw_reserved[4];
};

/**
 * sw_widget_get_state:
 * @self: a #SwWidget
 *
 * Gets the widget's current interaction state flags.
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: the current #SwWidgetState flags.
 */
SwWidgetState sw_widget_get_state (SwWidget *self);

/**
 * sw_widget_get_disabled:
 * @self: a #SwWidget
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: %TRUE if %SW_WIDGET_STATE_DISABLED is set.
 */
gboolean sw_widget_get_disabled (SwWidget *self);

/**
 * sw_widget_set_disabled:
 * @self: a #SwWidget
 * @disabled: whether @self should stop accepting input
 *
 * Sets or clears %SW_WIDGET_STATE_DISABLED. Per
 * docs/foundational/design-doc.md section 4, the disabled state is
 * exempt from the contrast minimums that apply to every other
 * interactive state, but must still be visually distinguishable from
 * enabled; that distinction is the default theme's responsibility, not
 * this function's.
 *
 * Thread-affinity: main-thread only.
 */
void sw_widget_set_disabled (SwWidget *self,
                              gboolean  disabled);

G_END_DECLS

#endif /* SW_WIDGET_H */
