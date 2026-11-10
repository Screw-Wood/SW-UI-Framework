# Proposal 0002: Lifecycle ownership model

**Status:** Accepted. Applies [proposal 0001](0001-dependency-ownership-policy.md) to `sw_application`/`sw_window`.

## 1. Summary

SW owns an Android-shaped application/window lifecycle: an enforced state machine, built on top of GTK4's application and widget signals, with a lifecycle-observer interface that other SW subsystems and application code can subscribe to. This is not a thin forwarding layer over GTK's signals — the enforced contract and the observer pattern are themselves the product, per [proposal 0001 §2.1](0001-dependency-ownership-policy.md#2-the-policy).

## 2. Why this is owned, not outsourced

Per the policy in proposal 0001: GTK4's own lifecycle (`GApplication`'s `startup`/`activate`/`open`/`shutdown`, and `GtkWidget`'s realize/map/show/hide/unmap/unrealize/destroy) already handles the platform mechanics — process registration, single-instance behavior, window/surface binding (see [RESEARCH.md §4](../foundational/RESEARCH.md#4-lifecycle-models-android-and-gtk4)). SW does not need to reimplement that mechanism.

What GTK4 does not provide is what makes Android's lifecycle a developer-experience win rather than just a set of callbacks:

- **An enforced contract.** Android throws `SuperNotCalledException` if an override doesn't chain up; GTK's signals carry no such guarantee. Without enforcement, every app is one missed callback away from a lifecycle bug.
- **A subscription model for third parties.** Jetpack's `LifecycleObserver` pattern lets components other than the Activity itself (ViewModel, LiveData, camera, location) react to lifecycle state without the Activity manually notifying each one. GTK gives no equivalent — a GTK app developer wires window state to every interested subsystem by hand today.

That manual wiring is exactly the "logic that isn't part of the business logic" SW exists to remove. So this is owned.

## 3. The state machine

`sw_window` (and by extension `sw_application`, which owns zero or more windows) exposes a fixed sequence of states, analogous to Android's `Activity` states, mapped onto GTK's underlying signals:

| SW state | Entered via (maps to GTK) | Analogous Android callback |
|---|---|---|
| `CREATED` | `sw_window` construction, before `realize` | `onCreate` |
| `SHOWN` | GTK `map`/`show` | `onStart` |
| `FOCUSED` | GTK `focus-in` (window-level) | `onResume` |
| `UNFOCUSED` | GTK `focus-out` (window-level) | `onPause` |
| `HIDDEN` | GTK `unmap`/`hide` | `onStop` |
| `DESTROYED` | GTK `unrealize`/`destroy` | `onDestroy` |

Rules, mirroring the Android guarantees in [RESEARCH.md §4](../foundational/RESEARCH.md#4-lifecycle-models-android-and-gtk4):

- States are reached in a fixed order going forward (`CREATED → SHOWN → FOCUSED`) and a fixed order going backward (`UNFOCUSED → HIDDEN → DESTROYED`), matching Android's forward/teardown symmetry.
- A window may cycle `FOCUSED ⇄ UNFOCUSED` and `SHOWN ⇄ HIDDEN` repeatedly without being destroyed, same as Android's `onPause`/`onResume` and `onStop`/`onRestart` cycles.
- Anything done entering `UNFOCUSED` must be safe to undo entering `FOCUSED` again — SW's own components (see §4) are expected to follow this symmetry, and the API documentation for each state transition should say so explicitly, the same way Android's docs do for `onPause`/`onResume`.
- `sw_window` enforces chain-up the way Android enforces `super.onX()`: application code that hooks a state transition must be unable to silently break the sequence for other observers. The exact enforcement mechanism (return value, required callback registration shape, or assertion in debug builds) is an implementation detail for the code that realizes this proposal, not fixed here — but the guarantee itself is not optional.

Application-level (`sw_application`) states are a thin layer on top of this: `startup`-equivalent and `shutdown`-equivalent map directly to GTK's `GApplication::startup`/`::shutdown`, which GTK already guarantees fire once, only on the primary instance.

## 4. The observer interface

SW defines an `SwLifecycleObserver`-shaped interface (naming and exact shape to be settled during implementation, not in this proposal) that any SW subsystem — or application code — can register against a `sw_window`'s lifecycle, analogous to Jetpack's `LifecycleObserver`. A widget or subsystem that cares about window state subscribes to it directly instead of `sw_window` having to know about every interested party individually.

This is the mechanism that makes lifecycle-awareness compositional rather than something every widget reimplements. Concretely, per the scope-change this proposal drives (§5), this is expected to back things like:

- A text-entry widget autosaving or committing its value on `UNFOCUSED`.
- An animation or transition controller cancelling in-flight animations on `HIDDEN`.
- A future networking/polling helper pausing on `UNFOCUSED` and resuming on `FOCUSED`.
- A dialog's modal focus-trap and dismiss-on-escape behavior, which SW owns outright under this model rather than leaving to raw GTK dialog behavior (resolving the Tier-4 scope question raised in discussion: dialogs are lifecycle-integrated, owned behavior, not deferred).

None of these specific integrations are being built by this proposal — it only establishes that the interface they'll use exists and is load-bearing from the start.

## 5. Consequences for component and MVP scope

- `sw_application`/`sw_window` are no longer a Tier-0 integration shim that other widgets sit beside. They are infrastructure every stateful widget is expected to compose with via the observer interface, from the first widgets implemented onward.
- Per-widget state (hover/pressed/focused/selected/disabled, [SYSTEM_DESIGN.md §10](../foundational/SYSTEM_DESIGN.md#10-input-focus-and-interaction)) is unaffected by this proposal — that's pointer/keyboard interaction state, not window lifecycle state. The two are related (a widget's focus-visible state depends on the window being `FOCUSED`) but are not the same state machine.
- Dialogs, popovers, and the navigation pattern — previously an open Tier-4 scope disagreement between IDEA.md §9 and SYSTEM_DESIGN.md's phase breakdown — are resolved as **owned, lifecycle-integrated behavior** per §4 above. Their *implementation timing* (which development phase they land in) is unchanged by this proposal; only the ownership question is settled.

## 6. Open implementation details

Left for the implementation itself, not blocking acceptance of this proposal:

- Exact `SwLifecycleObserver` API shape (callback struct, signal-based, or both).
- Whether chain-up enforcement is a runtime assertion, a return-value contract, or structural (observers can't "forget" to call through because there's nothing to chain up to).
- Whether `sw_application`-level state and `sw_window`-level state are exposed as one combined observer surface or two.

## 7. Related documents

- [Proposal 0001](0001-dependency-ownership-policy.md) — the policy this proposal applies.
- [RESEARCH.md §4](../foundational/RESEARCH.md#4-lifecycle-models-android-and-gtk4) — the Android/GTK4 lifecycle research behind this design.
- [SYSTEM_DESIGN.md §3](../foundational/SYSTEM_DESIGN.md#3-ownership-boundaries), [§10](../foundational/SYSTEM_DESIGN.md#10-input-focus-and-interaction), [§13](../foundational/SYSTEM_DESIGN.md#13-threading-and-lifecycle) — existing ownership, interaction, and lifecycle sections this proposal extends.
