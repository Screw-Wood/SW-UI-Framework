/* sw-window.c */

#define SW_COMPILATION
#include "sw-window.h"
#include "sw-application.h"

typedef struct
{
  SwWindowState  state;
  GPtrArray     *observers; /* element-type SwLifecycleObserver*, borrowed */
} SwWindowPrivate;

enum
{
  SIGNAL_STATE_CHANGED,
  N_SIGNALS,
};

static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_PRIVATE (SwWindow, sw_window, GTK_TYPE_WINDOW)

static void
sw_window_notify_observers (SwWindow      *self,
                             SwWindowState  state)
{
  SwWindowPrivate *priv = sw_window_get_instance_private (self);
  guint i;

  for (i = 0; i < priv->observers->len; i++)
    {
      SwLifecycleObserver *observer = g_ptr_array_index (priv->observers, i);

      switch (state)
        {
        case SW_WINDOW_STATE_SHOWN:
          sw_lifecycle_observer_on_shown (observer, self);
          break;
        case SW_WINDOW_STATE_FOCUSED:
          sw_lifecycle_observer_on_focused (observer, self);
          break;
        case SW_WINDOW_STATE_UNFOCUSED:
          sw_lifecycle_observer_on_unfocused (observer, self);
          break;
        case SW_WINDOW_STATE_HIDDEN:
          sw_lifecycle_observer_on_hidden (observer, self);
          break;
        case SW_WINDOW_STATE_DESTROYED:
          sw_lifecycle_observer_on_destroyed (observer, self);
          break;
        case SW_WINDOW_STATE_CREATED:
        default:
          /* CREATED has no observer callback: observers are registered
           * by application code after construction, so nothing could
           * have been listening yet (proposal 0002 section 3). */
          break;
        }
    }
}

/* Internal: the single place a state transition is allowed to happen.
 * This is what makes the chain-up guarantee in proposal 0002 section 3
 * structural rather than conventional: there is no override point for
 * an observer to skip, because SW_WINDOW calls every observer itself
 * from here, and this function is the only caller of
 * sw_window_notify_observers(). */
static void
sw_window_set_state (SwWindow      *self,
                      SwWindowState  state)
{
  SwWindowPrivate *priv = sw_window_get_instance_private (self);

  if (priv->state == state)
    return;

  priv->state = state;

  g_signal_emit (self, signals[SIGNAL_STATE_CHANGED], 0, state);
}

static void
sw_window_class_state_changed (SwWindow      *self,
                                SwWindowState  state)
{
  sw_window_notify_observers (self, state);
}

static void
sw_window_map (GtkWidget *widget)
{
  SwWindow *self = SW_WINDOW (widget);

  GTK_WIDGET_CLASS (sw_window_parent_class)->map (widget);

  sw_window_set_state (self, SW_WINDOW_STATE_SHOWN);
}

static void
sw_window_unmap (GtkWidget *widget)
{
  SwWindow *self = SW_WINDOW (widget);
  SwWindowPrivate *priv = sw_window_get_instance_private (self);

  /* Enforce the documented backward order (UNFOCUSED -> HIDDEN) even if
   * the windowing system delivers unmap without a preceding
   * notify::is-active toggle. */
  if (priv->state == SW_WINDOW_STATE_FOCUSED)
    sw_window_set_state (self, SW_WINDOW_STATE_UNFOCUSED);

  sw_window_set_state (self, SW_WINDOW_STATE_HIDDEN);

  GTK_WIDGET_CLASS (sw_window_parent_class)->unmap (widget);
}

static void
sw_window_destroy_signal (GtkWidget *widget,
                           gpointer   user_data)
{
  (void) user_data;

  /* GtkWidget::destroy, emitted by gtk_window_destroy(), has no class
   * vfunc slot in GTK4's GtkWidgetClass/GtkWindowClass to override, so
   * this is a plain signal connection rather than a chained-up vfunc
   * like sw_window_map()/sw_window_unmap() above. */
  sw_window_set_state (SW_WINDOW (widget), SW_WINDOW_STATE_DESTROYED);
}

static void
sw_window_notify_is_active (GObject    *object,
                             GParamSpec *pspec,
                             gpointer    user_data)
{
  SwWindow *self = SW_WINDOW (object);
  SwWindowPrivate *priv = sw_window_get_instance_private (self);

  (void) pspec;
  (void) user_data;

  /* A toplevel can become active before it has ever been mapped in some
   * backends' event ordering; only act once we are at least SHOWN, so
   * FOCUSED is never entered before SHOWN (proposal 0002 section 3's
   * fixed forward order). */
  if (priv->state == SW_WINDOW_STATE_CREATED)
    return;

  if (gtk_window_is_active (GTK_WINDOW (self)))
    {
      if (priv->state != SW_WINDOW_STATE_DESTROYED)
        sw_window_set_state (self, SW_WINDOW_STATE_FOCUSED);
    }
  else
    {
      if (priv->state == SW_WINDOW_STATE_FOCUSED)
        sw_window_set_state (self, SW_WINDOW_STATE_UNFOCUSED);
    }
}

static void
sw_window_finalize (GObject *object)
{
  SwWindow *self = SW_WINDOW (object);
  SwWindowPrivate *priv = sw_window_get_instance_private (self);

  g_clear_pointer (&priv->observers, g_ptr_array_unref);

  G_OBJECT_CLASS (sw_window_parent_class)->finalize (object);
}

static void
sw_window_class_init (SwWindowClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->finalize = sw_window_finalize;

  widget_class->map = sw_window_map;
  widget_class->unmap = sw_window_unmap;

  klass->state_changed = sw_window_class_state_changed;

  /**
   * SwWindow::state-changed:
   * @self: the #SwWindow
   * @state: the new #SwWindowState
   *
   * Emitted whenever @self transitions to a new lifecycle state. Most
   * application code should prefer #SwLifecycleObserver
   * (sw_window_add_lifecycle_observer()) over connecting to this signal
   * directly; this signal exists for callers for whom a plain GObject
   * signal connection is more convenient than implementing the
   * interface.
   */
  signals[SIGNAL_STATE_CHANGED] =
    g_signal_new ("state-changed",
                  G_TYPE_FROM_CLASS (klass),
                  G_SIGNAL_RUN_LAST,
                  G_STRUCT_OFFSET (SwWindowClass, state_changed),
                  NULL, NULL,
                  NULL,
                  G_TYPE_NONE, 1,
                  G_TYPE_INT);
}

static void
sw_window_init (SwWindow *self)
{
  SwWindowPrivate *priv = sw_window_get_instance_private (self);

  priv->state = SW_WINDOW_STATE_CREATED;
  priv->observers = g_ptr_array_new ();

  g_signal_connect (self, "notify::is-active",
                     G_CALLBACK (sw_window_notify_is_active), NULL);
  g_signal_connect (self, "destroy",
                     G_CALLBACK (sw_window_destroy_signal), NULL);
}

GtkWidget *
sw_window_new (SwApplication *application)
{
  g_return_val_if_fail (application == NULL || SW_IS_APPLICATION (application), NULL);

  return g_object_new (SW_TYPE_WINDOW,
                        "application", application,
                        NULL);
}

SwWindowState
sw_window_get_state (SwWindow *self)
{
  SwWindowPrivate *priv;

  g_return_val_if_fail (SW_IS_WINDOW (self), SW_WINDOW_STATE_DESTROYED);

  priv = sw_window_get_instance_private (self);
  return priv->state;
}

void
sw_window_add_lifecycle_observer (SwWindow            *self,
                                   SwLifecycleObserver *observer)
{
  SwWindowPrivate *priv;

  g_return_if_fail (SW_IS_WINDOW (self));
  g_return_if_fail (SW_IS_LIFECYCLE_OBSERVER (observer));

  priv = sw_window_get_instance_private (self);

  if (!g_ptr_array_find (priv->observers, observer, NULL))
    g_ptr_array_add (priv->observers, observer);
}

void
sw_window_remove_lifecycle_observer (SwWindow            *self,
                                      SwLifecycleObserver *observer)
{
  SwWindowPrivate *priv;

  g_return_if_fail (SW_IS_WINDOW (self));
  g_return_if_fail (SW_IS_LIFECYCLE_OBSERVER (observer));

  priv = sw_window_get_instance_private (self);

  g_ptr_array_remove_fast (priv->observers, observer);
}
