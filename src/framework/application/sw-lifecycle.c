/* sw-lifecycle.c */

#define SW_COMPILATION
#include "sw-lifecycle.h"

G_DEFINE_INTERFACE (SwLifecycleObserver, sw_lifecycle_observer, G_TYPE_OBJECT)

static void
sw_lifecycle_observer_default_init (SwLifecycleObserverInterface *iface)
{
  /* All vfuncs are optional (see sw-lifecycle.h); nothing to install
   * here. Kept as a named, documented default_init rather than left
   * implicit, per Architecture.md section 5's "documented before it
   * merges" standard applying to internal contracts too. */
  (void) iface;
}

#define SW_LIFECYCLE_OBSERVER_CALL(self, window, vfunc)                \
  G_STMT_START {                                                       \
    SwLifecycleObserverInterface *iface;                               \
    g_return_if_fail (SW_IS_LIFECYCLE_OBSERVER (self));                \
    iface = SW_LIFECYCLE_OBSERVER_GET_IFACE (self);                    \
    if (iface->vfunc != NULL)                                         \
      iface->vfunc (self, window);                                     \
  } G_STMT_END

void
sw_lifecycle_observer_on_shown (SwLifecycleObserver *self,
                                 SwWindow             *window)
{
  SW_LIFECYCLE_OBSERVER_CALL (self, window, on_shown);
}

void
sw_lifecycle_observer_on_focused (SwLifecycleObserver *self,
                                   SwWindow             *window)
{
  SW_LIFECYCLE_OBSERVER_CALL (self, window, on_focused);
}

void
sw_lifecycle_observer_on_unfocused (SwLifecycleObserver *self,
                                     SwWindow             *window)
{
  SW_LIFECYCLE_OBSERVER_CALL (self, window, on_unfocused);
}

void
sw_lifecycle_observer_on_hidden (SwLifecycleObserver *self,
                                  SwWindow             *window)
{
  SW_LIFECYCLE_OBSERVER_CALL (self, window, on_hidden);
}

void
sw_lifecycle_observer_on_destroyed (SwLifecycleObserver *self,
                                     SwWindow             *window)
{
  SW_LIFECYCLE_OBSERVER_CALL (self, window, on_destroyed);
}

#undef SW_LIFECYCLE_OBSERVER_CALL
