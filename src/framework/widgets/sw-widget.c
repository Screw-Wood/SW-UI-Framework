/* sw-widget.c */

#define SW_COMPILATION
#include "sw-widget.h"
#include "sw-widget-private.h"

typedef struct
{
  SwWidgetState state;
} SwWidgetPrivate;

enum
{
  SIGNAL_STATE_CHANGED,
  N_SIGNALS,
};

static guint signals[N_SIGNALS];

/* Abstract: per sw-widget.h, SwWidget is a shared base and is not meant
 * to be instantiated directly. docs/proposal/0003-day-zero-widget-set.md
 * section 4 describes it as "(base, not directly instantiable)". */
G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (SwWidget, sw_widget, GTK_TYPE_WIDGET)

static const struct
{
  SwWidgetState flag;
  const char   *css_class;
} sw_widget_state_css[] = {
  { SW_WIDGET_STATE_HOVER,    "state-hover" },
  { SW_WIDGET_STATE_PRESSED,  "state-pressed" },
  { SW_WIDGET_STATE_FOCUSED,  "state-focused" },
  { SW_WIDGET_STATE_SELECTED, "state-selected" },
  { SW_WIDGET_STATE_DISABLED, "state-disabled" },
};

static void
sw_widget_apply_state (SwWidget *self,
                        SwWidgetState state)
{
  guint i;

  for (i = 0; i < G_N_ELEMENTS (sw_widget_state_css); i++)
    {
      if (state & sw_widget_state_css[i].flag)
        gtk_widget_add_css_class (GTK_WIDGET (self), sw_widget_state_css[i].css_class);
      else
        gtk_widget_remove_css_class (GTK_WIDGET (self), sw_widget_state_css[i].css_class);
    }
}

/* The single place SwWidgetState is allowed to change (sw-widget-private.h),
 * so subclasses (SwButton, SwEntry, ...) go through one consistent path
 * instead of poking priv->state directly, and so every state change
 * emits "state-changed" exactly once. Not public API: only SW's own
 * widget implementations, not application code, call this. */
void
_sw_widget_set_state (SwWidget      *self,
                               SwWidgetState  state,
                               gboolean       set)
{
  SwWidgetPrivate *priv = sw_widget_get_instance_private (self);
  SwWidgetState new_state = set ? (priv->state | state) : (priv->state & ~state);

  if (new_state == priv->state)
    return;

  priv->state = new_state;

  g_signal_emit (self, signals[SIGNAL_STATE_CHANGED], 0, new_state);
}

void
sw_widget_set_disabled (SwWidget *self,
                         gboolean  disabled)
{
  g_return_if_fail (SW_IS_WIDGET (self));

  _sw_widget_set_state (self, SW_WIDGET_STATE_DISABLED, disabled);
  gtk_widget_set_sensitive (GTK_WIDGET (self), !disabled);
}

gboolean
sw_widget_get_disabled (SwWidget *self)
{
  g_return_val_if_fail (SW_IS_WIDGET (self), FALSE);

  return (sw_widget_get_state (self) & SW_WIDGET_STATE_DISABLED) != 0;
}

SwWidgetState
sw_widget_get_state (SwWidget *self)
{
  SwWidgetPrivate *priv;

  g_return_val_if_fail (SW_IS_WIDGET (self), SW_WIDGET_STATE_NORMAL);

  priv = sw_widget_get_instance_private (self);
  return priv->state;
}

static void
sw_widget_motion_enter (GtkEventControllerMotion *controller,
                         double                    x,
                         double                    y,
                         gpointer                  user_data)
{
  (void) controller;
  (void) x;
  (void) y;

  _sw_widget_set_state (SW_WIDGET (user_data), SW_WIDGET_STATE_HOVER, TRUE);
}

static void
sw_widget_motion_leave (GtkEventControllerMotion *controller,
                         gpointer                  user_data)
{
  (void) controller;

  _sw_widget_set_state (SW_WIDGET (user_data), SW_WIDGET_STATE_HOVER, FALSE);
}

static void
sw_widget_focus_enter (GtkEventControllerFocus *controller,
                        gpointer                 user_data)
{
  (void) controller;

  _sw_widget_set_state (SW_WIDGET (user_data), SW_WIDGET_STATE_FOCUSED, TRUE);
}

static void
sw_widget_focus_leave (GtkEventControllerFocus *controller,
                        gpointer                 user_data)
{
  (void) controller;

  _sw_widget_set_state (SW_WIDGET (user_data), SW_WIDGET_STATE_FOCUSED, FALSE);
}

static void
sw_widget_class_state_changed (SwWidget      *self,
                                SwWidgetState  state)
{
  sw_widget_apply_state (self, state);
}

static void
sw_widget_class_init (SwWidgetClass *klass)
{
  /**
   * SwWidget::state-changed:
   * @self: the #SwWidget
   * @state: the new #SwWidgetState flags
   *
   * Emitted whenever @self's interaction state changes.
   */
  signals[SIGNAL_STATE_CHANGED] =
    g_signal_new ("state-changed",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (SwWidgetClass, state_changed),
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 1,
                  G_TYPE_UINT);

  klass->state_changed = sw_widget_class_state_changed;

  /* No accessible role is set here: SW_WIDGET is abstract and has no
   * single correct role. Every concrete subclass must call
   * gtk_widget_class_set_accessible_role() in its own class_init, per
   * docs/proposal/0003-day-zero-widget-set.md section 4. */
}

static void
sw_widget_init (SwWidget *self)
{
  GtkEventController *motion;
  GtkEventController *focus;

  motion = gtk_event_controller_motion_new ();
  g_signal_connect (motion, "enter", G_CALLBACK (sw_widget_motion_enter), self);
  g_signal_connect (motion, "leave", G_CALLBACK (sw_widget_motion_leave), self);
  gtk_widget_add_controller (GTK_WIDGET (self), motion);

  focus = gtk_event_controller_focus_new ();
  g_signal_connect (focus, "enter", G_CALLBACK (sw_widget_focus_enter), self);
  g_signal_connect (focus, "leave", G_CALLBACK (sw_widget_focus_leave), self);
  gtk_widget_add_controller (GTK_WIDGET (self), focus);
}
