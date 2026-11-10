/* sw-button.c */

#define SW_COMPILATION
#include "sw-button.h"
#include "sw-widget-private.h"

struct _SwButton
{
  SwWidget    parent_instance;

  GtkWidget  *inner_label; /* owned via gtk_widget_set_parent() */
};

enum
{
  SIGNAL_CLICKED,
  N_SIGNALS,
};

static guint signals[N_SIGNALS];

G_DEFINE_TYPE (SwButton, sw_button, SW_TYPE_WIDGET)

static void
sw_button_pressed (GtkGestureClick *gesture,
                    int              n_press,
                    double           x,
                    double           y,
                    gpointer         user_data)
{
  (void) gesture;
  (void) n_press;
  (void) x;
  (void) y;

  _sw_widget_set_state (SW_WIDGET (user_data), SW_WIDGET_STATE_PRESSED, TRUE);
}

static void
sw_button_released (GtkGestureClick *gesture,
                     int              n_press,
                     double           x,
                     double           y,
                     gpointer         user_data)
{
  SwButton *self = SW_BUTTON (user_data);

  (void) gesture;
  (void) n_press;
  (void) x;
  (void) y;

  _sw_widget_set_state (SW_WIDGET (self), SW_WIDGET_STATE_PRESSED, FALSE);

  /* GtkGestureClick's "released" only fires when press and release
   * belong to the same recognized click, so no separate bounds check
   * is needed here (unlike a raw button-release event). */
  g_signal_emit (self, signals[SIGNAL_CLICKED], 0);
}

static void
sw_button_gesture_cancel (GtkGesture       *gesture,
                           GdkEventSequence *sequence,
                           gpointer          user_data)
{
  (void) gesture;
  (void) sequence;

  _sw_widget_set_state (SW_WIDGET (user_data), SW_WIDGET_STATE_PRESSED, FALSE);
}

static void
sw_button_dispose (GObject *object)
{
  SwButton *self = SW_BUTTON (object);

  g_clear_pointer (&self->inner_label, gtk_widget_unparent);

  G_OBJECT_CLASS (sw_button_parent_class)->dispose (object);
}

static void
sw_button_class_init (SwButtonClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = sw_button_dispose;

  gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BIN_LAYOUT);
  gtk_widget_class_set_accessible_role (widget_class, GTK_ACCESSIBLE_ROLE_BUTTON);
  gtk_widget_class_set_css_name (widget_class, "sw-button");

  /**
   * SwButton::clicked:
   * @self: the #SwButton
   *
   * Emitted when the button is activated, either by a pointer
   * press-and-release inside the widget or by Space/Return while the
   * button has keyboard focus (docs/proposal/0003-day-zero-widget-set.md
   * section 4's "full interaction contract").
   */
  signals[SIGNAL_CLICKED] =
    g_signal_new ("clicked",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_FIRST,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 0);

  /* Keyboard activation: Space/Return/Numpad-Enter emit the same
   * "clicked" signal as a pointer click, satisfying the day-zero
   * keyboard-activation requirement without a separate code path. */
  gtk_widget_class_add_binding_signal (widget_class, GDK_KEY_space, 0, "clicked", NULL);
  gtk_widget_class_add_binding_signal (widget_class, GDK_KEY_Return, 0, "clicked", NULL);
  gtk_widget_class_add_binding_signal (widget_class, GDK_KEY_KP_Enter, 0, "clicked", NULL);
}

static void
sw_button_init (SwButton *self)
{
  GtkGesture *click;

  gtk_widget_set_focusable (GTK_WIDGET (self), TRUE);
  gtk_widget_set_cursor_from_name (GTK_WIDGET (self), "pointer");

  self->inner_label = gtk_label_new (NULL);
  gtk_widget_set_parent (self->inner_label, GTK_WIDGET (self));

  click = gtk_gesture_click_new ();
  gtk_gesture_single_set_button (GTK_GESTURE_SINGLE (click), GDK_BUTTON_PRIMARY);
  g_signal_connect (click, "pressed", G_CALLBACK (sw_button_pressed), self);
  g_signal_connect (click, "released", G_CALLBACK (sw_button_released), self);
  g_signal_connect (click, "cancel", G_CALLBACK (sw_button_gesture_cancel), self);
  gtk_widget_add_controller (GTK_WIDGET (self), GTK_EVENT_CONTROLLER (click));
}

GtkWidget *
sw_button_new (const char *label)
{
  GtkWidget *button = g_object_new (SW_TYPE_BUTTON, NULL);

  sw_button_set_label (SW_BUTTON (button), label);

  return button;
}

void
sw_button_set_label (SwButton   *self,
                      const char *label)
{
  g_return_if_fail (SW_IS_BUTTON (self));

  gtk_label_set_text (GTK_LABEL (self->inner_label), label != NULL ? label : "");
  gtk_accessible_update_property (GTK_ACCESSIBLE (self),
                                   GTK_ACCESSIBLE_PROPERTY_LABEL, label != NULL ? label : "",
                                   -1);
}

const char *
sw_button_get_label (SwButton *self)
{
  g_return_val_if_fail (SW_IS_BUTTON (self), NULL);

  return gtk_label_get_text (GTK_LABEL (self->inner_label));
}
