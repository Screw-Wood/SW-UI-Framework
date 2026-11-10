# 0001: self-removing lifecycle observer makes the next observer skip a callback

**Status:** Open
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built locally with GTK 4.14.5 / Meson, Ubuntu packages)
**Affects:** `src/framework/application/sw-lifecycle.c` (observer notification), `SwLifecycleObserver`

## What happens

When a registered `SwLifecycleObserver` calls `sw_window_remove_lifecycle_observer` on itself from inside its own callback for a given transition, the observer registered immediately after it in the list does not receive the callback for that same transition. Later transitions notify it normally.

## What you expected

[Proposal 0002 section 3](../../proposal/0002-lifecycle-ownership-model.md#3-the-state-machine) and [section 4](../../proposal/0002-lifecycle-ownership-model.md#4-the-observer-interface) require every registered observer to receive every transition, and `docs/Implementation/README.md`'s "structural chain-up guarantee" language assumes the same: self-removal during notification should not affect delivery to any other observer for that same transition.

## How to reproduce

Register three observers (A, B, C) on the same `SwWindow`. In A's callback for a transition, call `sw_window_remove_lifecycle_observer` on A itself. Trigger the transition. B does not receive the callback; C does.

## Notes

Reproduced with a throwaway probe program against a scratch copy of the source, not committed to the tree. Reading the notification loop in `sw-lifecycle.c`, this looks like an in-place iteration over the observer list that does not account for the list shrinking by one when the current entry removes itself: the index used to step to "the next observer" ends up pointing past the one that should be next. A copy-before-iterating, or an iteration style safe against concurrent removal, would fix it. No fix has been made to `src/`.
