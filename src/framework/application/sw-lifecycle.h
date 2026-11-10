/* sw-lifecycle.h
 *
 * The lifecycle-observer interface from
 * docs/proposal/0002-lifecycle-ownership-model.md section 4: the
 * mechanism that lets any SW subsystem or application object react to
 * an #SwWindow's state without #SwWindow having to know about every
 * interested party individually.
 *
 * Chain-up enforcement (proposal 0002 section 3) is structural here:
 * #SwWindow calls every registered observer's vfunc itself; there is no
 * override-and-chain-up step for an observer to forget, unlike
 * Android's `super.onX()` convention this interface is modeled on.
 */

#ifndef SW_LIFECYCLE_H
#define SW_LIFECYCLE_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <glib-object.h>

G_BEGIN_DECLS

#define SW_TYPE_LIFECYCLE_OBSERVER (sw_lifecycle_observer_get_type ())

G_DECLARE_INTERFACE (SwLifecycleObserver, sw_lifecycle_observer, SW, LIFECYCLE_OBSERVER, GObject)

/* Forward-declared rather than included: sw-window.h includes this
 * header to declare sw_window_add_lifecycle_observer(), so this header
 * cannot include sw-window.h back without a cycle. Observers only need
 * a pointer, not SwWindow's full declaration. */
typedef struct _SwWindow SwWindow;

/**
 * SwLifecycleObserverInterface:
 * @parent_iface: the parent interface
 * @on_shown: called when the observed window enters
 *   %SW_WINDOW_STATE_SHOWN (analogous to Android's `onStart`).
 * @on_focused: called when the observed window enters
 *   %SW_WINDOW_STATE_FOCUSED (analogous to Android's `onResume`).
 * @on_unfocused: called when the observed window enters
 *   %SW_WINDOW_STATE_UNFOCUSED (analogous to Android's `onPause`).
 *   Per proposal 0002 section 3, anything done here must be safe to
 *   undo in the following @on_focused.
 * @on_hidden: called when the observed window enters
 *   %SW_WINDOW_STATE_HIDDEN (analogous to Android's `onStop`).
 * @on_destroyed: called when the observed window enters
 *   %SW_WINDOW_STATE_DESTROYED (analogous to Android's `onDestroy`).
 *   The observer must not call back into @window after this returns.
 *
 * All vfuncs are optional; an implementation only needs to provide the
 * ones it cares about.
 */
struct _SwLifecycleObserverInterface
{
  GTypeInterface parent_iface;

  void (* on_shown)     (SwLifecycleObserver *self,
                          SwWindow             *window);
  void (* on_focused)   (SwLifecycleObserver *self,
                          SwWindow             *window);
  void (* on_unfocused) (SwLifecycleObserver *self,
                          SwWindow             *window);
  void (* on_hidden)    (SwLifecycleObserver *self,
                          SwWindow             *window);
  void (* on_destroyed) (SwLifecycleObserver *self,
                          SwWindow             *window);
};

void sw_lifecycle_observer_on_shown     (SwLifecycleObserver *self,
                                          SwWindow             *window);
void sw_lifecycle_observer_on_focused   (SwLifecycleObserver *self,
                                          SwWindow             *window);
void sw_lifecycle_observer_on_unfocused (SwLifecycleObserver *self,
                                          SwWindow             *window);
void sw_lifecycle_observer_on_hidden    (SwLifecycleObserver *self,
                                          SwWindow             *window);
void sw_lifecycle_observer_on_destroyed (SwLifecycleObserver *self,
                                          SwWindow             *window);

G_END_DECLS

#endif /* SW_LIFECYCLE_H */
