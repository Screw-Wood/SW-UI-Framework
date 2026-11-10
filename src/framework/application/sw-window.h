/* sw-window.h
 *
 * SwWindow owns the Android-shaped application/window lifecycle
 * accepted in docs/proposal/0002-lifecycle-ownership-model.md: an
 * enforced state machine built on top of GTK4's own `GtkWindow`
 * signals, plus the #SwLifecycleObserver subscription point declared
 * in sw-lifecycle.h.
 */

#ifndef SW_WINDOW_H
#define SW_WINDOW_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <gtk/gtk.h>

#include "sw-lifecycle.h"

G_BEGIN_DECLS

/* Forward-declared rather than included: sw-application.h includes this
 * header for sw_application's window-creation convenience, so this
 * header cannot include sw-application.h back without a cycle. */
typedef struct _SwApplication SwApplication;

#define SW_TYPE_WINDOW (sw_window_get_type ())

G_DECLARE_DERIVABLE_TYPE (SwWindow, sw_window, SW, WINDOW, GtkWindow)

/**
 * SwWindowState:
 * @SW_WINDOW_STATE_CREATED: the window has been constructed but not yet
 *   realized. Entered via construction, before GTK's `realize`.
 *   Analogous to Android's `onCreate`.
 * @SW_WINDOW_STATE_SHOWN: the window is mapped on screen. Entered via
 *   GTK's `map`/`show`. Analogous to Android's `onStart`.
 * @SW_WINDOW_STATE_FOCUSED: the window is mapped and has input focus.
 *   Entered via GTK's window-level `notify::is-active` becoming %TRUE.
 *   Analogous to Android's `onResume`.
 * @SW_WINDOW_STATE_UNFOCUSED: the window is mapped but does not have
 *   input focus. Entered via `notify::is-active` becoming %FALSE.
 *   Analogous to Android's `onPause`.
 * @SW_WINDOW_STATE_HIDDEN: the window is unmapped but not destroyed.
 *   Entered via GTK's `unmap`/`hide`. Analogous to Android's `onStop`.
 * @SW_WINDOW_STATE_DESTROYED: the window has been destroyed. Entered
 *   via GTK's `unrealize`/`destroy`. Analogous to Android's `onDestroy`.
 *
 * The states a #SwWindow moves through, per
 * docs/proposal/0002-lifecycle-ownership-model.md section 3. States are
 * reached in a fixed order going forward
 * (%SW_WINDOW_STATE_CREATED -> %SW_WINDOW_STATE_SHOWN ->
 * %SW_WINDOW_STATE_FOCUSED) and a fixed order going backward
 * (%SW_WINDOW_STATE_UNFOCUSED -> %SW_WINDOW_STATE_HIDDEN ->
 * %SW_WINDOW_STATE_DESTROYED). A window may cycle between
 * %SW_WINDOW_STATE_FOCUSED and %SW_WINDOW_STATE_UNFOCUSED, and between
 * %SW_WINDOW_STATE_SHOWN and %SW_WINDOW_STATE_HIDDEN, repeatedly
 * without being destroyed.
 */
typedef enum
{
  SW_WINDOW_STATE_CREATED,
  SW_WINDOW_STATE_SHOWN,
  SW_WINDOW_STATE_FOCUSED,
  SW_WINDOW_STATE_UNFOCUSED,
  SW_WINDOW_STATE_HIDDEN,
  SW_WINDOW_STATE_DESTROYED,
} SwWindowState;

struct _SwWindowClass
{
  GtkWindowClass parent_class;

  /**
   * SwWindowClass::state_changed:
   * @self: the #SwWindow
   * @state: the new #SwWindowState
   *
   * Class handler for the #SwWindow::state-changed signal. Subclasses
   * overriding this must chain up, since this is where registered
   * #SwLifecycleObserver instances are notified
   * (docs/proposal/0002-lifecycle-ownership-model.md section 3's
   * chain-up guarantee).
   */
  void (* state_changed) (SwWindow      *self,
                           SwWindowState  state);

  /* Reserved for future virtual methods; keeps ABI room per
   * Architecture.md section 2's parallel-installability policy. */
  gpointer _sw_reserved[4];
};

/**
 * sw_window_new:
 * @application: (nullable): the #SwApplication this window belongs to
 *
 * Creates a new #SwWindow in %SW_WINDOW_STATE_CREATED.
 *
 * Ownership: transfer-full. The returned window is floating, like any
 * `GtkWidget`; the caller owns the initial reference.
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: (transfer full): a new #SwWindow.
 */
GtkWidget *sw_window_new (SwApplication *application);

/**
 * sw_window_get_state:
 * @self: a #SwWindow
 *
 * Gets the window's current lifecycle state.
 *
 * Thread-affinity: main-thread only.
 *
 * Returns: the current #SwWindowState.
 */
SwWindowState sw_window_get_state (SwWindow *self);

/**
 * sw_window_add_lifecycle_observer:
 * @self: a #SwWindow
 * @observer: (transfer none): an object implementing
 *   #SwLifecycleObserver
 *
 * Registers @observer to be notified of @self's lifecycle-state
 * transitions, per docs/proposal/0002-lifecycle-ownership-model.md
 * section 4. @self does not take a reference on @observer; the caller
 * must call sw_window_remove_lifecycle_observer() before @observer is
 * finalized.
 *
 * Ownership: transfer-none on @observer.
 *
 * Thread-affinity: main-thread only.
 */
void sw_window_add_lifecycle_observer (SwWindow             *self,
                                        SwLifecycleObserver  *observer);

/**
 * sw_window_remove_lifecycle_observer:
 * @self: a #SwWindow
 * @observer: (transfer none): a previously-registered observer
 *
 * Unregisters @observer added with sw_window_add_lifecycle_observer().
 * A no-op if @observer is not currently registered.
 *
 * Ownership: transfer-none on @observer.
 *
 * Thread-affinity: main-thread only.
 */
void sw_window_remove_lifecycle_observer (SwWindow             *self,
                                           SwLifecycleObserver  *observer);

G_END_DECLS

#endif /* SW_WINDOW_H */
