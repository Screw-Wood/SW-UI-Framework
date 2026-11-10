# 0005: SwEntry::committed also fires on the entry's own focus-out, not only on window UNFOCUSED

**Status:** Open
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built locally with GTK 4.14.5 / Meson, Ubuntu packages)
**Affects:** `src/framework/widgets/sw-entry.c`, `src/framework/widgets/sw-entry.h`

## What happens

`SwEntry` emits `committed` when the entry itself loses keyboard focus (for example, Tab moves focus to a sibling widget in the same still-focused window), in addition to emitting it when the owning `SwWindow` transitions to `SW_WINDOW_STATE_UNFOCUSED`.

## What you expected

`sw-entry.h`'s own documentation comment says `committed` is "emitted with the entry's current text when its owning SwWindow transitions to SW_WINDOW_STATE_UNFOCUSED" — only that, per the lifecycle-observer integration in [proposal 0002 section 4](../../proposal/0002-lifecycle-ownership-model.md#4-the-observer-interface). An extra emission on the entry's own GTK focus-out is not part of that contract and means consumers relying on "one commit per window-unfocus" see an extra one per Tab-away.

## How to reproduce

1. Build and run a program with an `SwEntry` and at least one other focusable widget in the same window.
2. Type into the entry, then Tab to the other widget (window stays focused/active throughout).
3. Observed: `committed` fires on the Tab-away. Expected: no `committed` until the window itself loses focus.

## Notes

Looks like `sw-entry.c` wires a commit on the internal text widget's own `focus-out-event`/equivalent signal, in addition to the intended hook into the `SwLifecycleObserver` callback for `UNFOCUSED`. Confirmed separately in the "What works" checks that the intended `UNFOCUSED`-driven commit does work and that `sw_entry_set_text` does not spuriously commit — this is specifically the extra, undocumented emission path. No fix has been made to `src/`.
