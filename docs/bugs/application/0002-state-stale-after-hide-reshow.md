# 0002: window state stays SHOWN, not FOCUSED, after hide and re-show

**Status:** Open — needs re-checking under a real window manager
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built locally with GTK 4.14.5 / Meson, Ubuntu packages; observed under Xvfb driven with `xdotool`, no window manager running)
**Affects:** `src/framework/application/sw-window.c` (lifecycle state tracking)

## What happens

After a window reaches `SW_WINDOW_STATE_HIDDEN` and is shown again, the state reported by the window stays at `SW_WINDOW_STATE_SHOWN` even though the window is active (has pointer/keyboard focus) on screen. The forward and backward orderings described in [proposal 0002 section 3](../../proposal/0002-lifecycle-ownership-model.md#3-the-state-machine) (`CREATED → SHOWN → FOCUSED → UNFOCUSED → HIDDEN → DESTROYED`) hold for the *first* show, but a hide/re-show cycle does not re-enter `FOCUSED`.

## What you expected

[Proposal 0002 section 3](../../proposal/0002-lifecycle-ownership-model.md#3-the-state-machine) says `SHOWN ⇄ HIDDEN` can cycle repeatedly without destroying the window; it does not say the window should get stuck at `SHOWN` instead of continuing on to `FOCUSED` once it actually has focus again.

## How to reproduce

1. Create and show an `SwWindow`; confirm it reaches `FOCUSED`.
2. Hide it (`HIDDEN`).
3. Show it again and give it focus.
4. Observed: state reads `SHOWN`. Expected: state reads `FOCUSED`.

## Notes

This was only exercised under Xvfb with no window manager, which is an unusual environment for "give it focus" — there is no window manager present to generate the focus-in event the state machine may be waiting for, so this could be an artifact of the test environment rather than a real bug. It needs to be re-checked under a real desktop session (a window manager actually running) before being trusted. Linked from [STAGES.md Stage 2](../../Implementation/STAGES.md#stage-2--lifecycle-and-observer-tests), whose "Decisions needed" already flagged headless focus-driving as unresolved.
