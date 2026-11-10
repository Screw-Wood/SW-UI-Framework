/* sw-entry.c */

#define SW_COMPILATION
#include "sw-entry.h"
#include "sw-widget-private.h"
#include "application/sw-lifecycle.h"
#include "application/sw-window.h"

enum
{
  SIGNAL_COMMITTED,
  N_SIGNALS,
};

static guint signals[N_SIGNALS];

static void sw_entry_lifecycle_observer_iface_init (SwLifecycleObserverInterface *iface);

struct _SwEntry
{
  SwWidget   parent_instance;

  GtkWidget *inner_text; /* GtkText, owned via gtk_widget_set_parent() */
  SwWindow  *observed_window; /* borrowed; non-NULL only while realized */
};

G_DEFINE_TYPE_WITH_CODE (SwEntry, sw_entry, SW_TYPE_WIDGET,
                          G_IMPLEMENT_INTERFACE (SW_TYPE_LIFECYCLE_OBSERVER,
                                                  sw_entry_lifecycle_observer_iface_init))

static void
sw_entry_commit (SwEntry *self)
{
  g_signal_emit (self, signals[SIGNAL_COMMITTED], 0, sw_entry_get_text (self));
}

static void
sw_entry_on_unfocused (SwLifecycleObserver *observer,
                        SwWindow             *window)
{
  (void) window;

  /* This is the concrete integration named in
   * docs/proposal/0002-lifecycle-ownership-model.md section 4: "a
   * text-entry widget autosaving or committing its value on
   * UNFOCUSED". It commits on the *window's* unfocus, which is
   * distinct from this entry losing its own keyboard focus (SwWidget's
   * SW_WIDGET_STATE_FOCUSED) — both are legitimate commit triggers, but
   * only the window-level one depends on the lifecycle-observer
   * interface, so that is what day zero proves. */
  sw_entry_commit (SW_ENTRY (observer));
}

static void
sw_entry_lifecycle_observer_iface_init (SwLifecycleObserverInterface *iface)
{
  iface->on_unfocused = sw_entry_on_unfocused;
}

static void
sw_entry_realize (GtkWidget *widget)
{
  SwEntry *self = SW_ENTRY (widget);
  GtkWidget *root;

  GTK_WIDGET_CLASS (sw_entry_parent_class)->realize (widget);

  root = GTK_WIDGET (gtk_widget_get_root (widget));
  if (SW_IS_WINDOW (root))
    {
      self->observed_window = SW_WINDOW (root);
      sw_window_add_lifecycle_observer (self->observed_window,
                                         SW_LIFECYCLE_OBSERVER (self));
    }
}

static void
sw_entry_unrealize (GtkWidget *widget)
{
  SwEntry *self = SW_ENTRY (widget);

  if (self->observed_window != NULL)
    {
      sw_window_remove_lifecycle_observer (self->observed_window,
                                            SW_LIFECYCLE_OBSERVER (self));
      self->observed_window = NULL;
    }

  GTK_WIDGET_CLASS (sw_entry_parent_class)->unrealize (widget);
}

static void
sw_entry_focus_leave (GtkEventControllerFocus *controller,
                       gpointer                  user_data)
{
  (void) controller;

  /* Also commit on the entry's own focus-out, not only on window
   * unfocus: tabbing to the next widget in the same, still-focused
   * window should commit too. The lifecycle-observer path in
   * sw_entry_on_unfocused() covers the window losing focus entirely
   * (switching applications, for example); this covers losing focus
   * within the window. Both call the same sw_entry_commit(). */
  sw_entry_commit (SW_ENTRY (user_data));
}

static void
sw_entry_dispose (GObject *object)
{
  SwEntry *self = SW_ENTRY (object);

  g_clear_pointer (&self->inner_text, gtk_widget_unparent);

  G_OBJECT_CLASS (sw_entry_parent_class)->dispose (object);
}

static void
sw_entry_class_init (SwEntryClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = sw_entry_dispose;

  widget_class->realize = sw_entry_realize;
  widget_class->unrealize = sw_entry_unrealize;

  gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BIN_LAYOUT);
  gtk_widget_class_set_accessible_role (widget_class, GTK_ACCESSIBLE_ROLE_TEXT_BOX);
  gtk_widget_class_set_css_name (widget_class, "sw-entry");

  /**
   * SwEntry::committed:
   * @self: the #SwEntry
   * @text: the committed text
   *
   * Emitted when @self's text is committed: on losing keyboard focus,
   * or when the owning #SwWindow becomes
   * %SW_WINDOW_STATE_UNFOCUSED (docs/proposal/0002-lifecycle-ownership-model.md
   * section 4).
   */
  signals[SIGNAL_COMMITTED] =
    g_signal_new ("committed",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  0,
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 1,
                  G_TYPE_STRING);
}

static void
sw_entry_init (SwEntry *self)
{
  GtkEventController *focus;

  gtk_widget_set_focusable (GTK_WIDGET (self), TRUE);

  /* GtkText, not GtkEntry: GtkEntry adds its own frame/icon chrome that
   * SW's own theme would have to fight. GtkText is the bare editable
   * text buffer GtkEntry itself is built on, which is exactly the
   * "outsource text editing and IME mechanics to GTK/Pango, own the
   * surrounding widget and visual contract" split from
   * docs/proposal/0001-dependency-ownership-policy.md section 3. */
  self->inner_text = gtk_text_new ();
  gtk_widget_set_parent (self->inner_text, GTK_WIDGET (self));
  gtk_widget_set_hexpand (self->inner_text, TRUE);

  focus = gtk_event_controller_focus_new ();
  g_signal_connect (focus, "leave", G_CALLBACK (sw_entry_focus_leave), self);
  gtk_widget_add_controller (self->inner_text, focus);
}

GtkWidget *
sw_entry_new (void)
{
  return g_object_new (SW_TYPE_ENTRY, NULL);
}

void
sw_entry_set_text (SwEntry    *self,
                    const char *text)
{
  g_return_if_fail (SW_IS_ENTRY (self));

  gtk_editable_set_text (GTK_EDITABLE (self->inner_text), text != NULL ? text : "");
}

const char *
sw_entry_get_text (SwEntry *self)
{
  g_return_val_if_fail (SW_IS_ENTRY (self), NULL);

  return gtk_editable_get_text (GTK_EDITABLE (self->inner_text));
}
