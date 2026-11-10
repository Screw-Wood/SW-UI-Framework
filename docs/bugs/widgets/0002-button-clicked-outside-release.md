# 0002: SwButton emits clicked when released outside the button

**Status:** Open
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built locally with GTK 4.14.5 / Meson, Ubuntu packages; driven with real pointer events via `xdotool` on Xvfb)
**Affects:** `src/framework/widgets/sw-button.c`

## What happens

Press the pointer down on an `SwButton`, drag outside its bounds, and release there: `clicked` still fires. The standard "press, drag off, release" cancel gesture does not cancel.

## What you expected

Standard button behaviour, and the pointer-interaction item in [STAGES.md Stage 1](../../Implementation/STAGES.md#stage-1--widget-tests) ("press, release, and cancel"), is that releasing outside the widget after pressing inside it cancels the click rather than emitting it.

## How to reproduce

1. Build and run any program constructing an `SwButton`.
2. Press the pointer down inside the button.
3. Move the pointer outside the button's bounds while still held down.
4. Release.
5. Observed: `clicked` fires. Expected: no `clicked`, and the pressed visual state clears.

## Notes

The pressed/hover states mentioned in the "What works" summary for ordinary clicks are correct; this is specifically the cancel-on-drag-off path. Likely the click gesture's "released" handler doesn't check whether the release point is still within the button's allocation before emitting `clicked`. No fix has been made to `src/`.
