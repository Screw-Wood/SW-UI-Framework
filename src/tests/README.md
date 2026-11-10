# src/tests

Tests for `src/framework`, following the testing strategy proposed in [docs/foundational/SYSTEM_DESIGN.md section 15](../../docs/foundational/SYSTEM_DESIGN.md#15-testing-strategy).

## Current layout

```text
src/tests/
  test-core.c     unit coverage for core/: sw_init, sw_version_string, sw_check_version, sw_error_quark, metrics tokens
  meson.build
```

Planned tiers not yet present, per SYSTEM_DESIGN.md section 15:

```text
  widget/         minimum/natural size, layout and resize behavior, pointer/keyboard interaction, focus traversal
  accessibility/  accessible role and name, state exposure, label relationships, keyboard-only operation
  visual/         light/dark themes, scale factors, interaction states, window sizes
  integration/    Wayland/X11 sessions, packaging, shutdown and object lifecycle, input methods and clipboard
```

A component should not be considered complete merely because it renders — see SYSTEM_DESIGN.md section 15 and [IDEA.md section 9](../../docs/foundational/IDEA.md#9-what-a-useful-first-release-might-contain). Sizing, states, keyboard behavior, accessibility semantics, and scaling behavior each need test coverage before a widget is treated as done; per [Architecture.md section 4](../../docs/foundational/Architecture.md#4-testing-gate-for-release), this is a release gate, not aspirational.

## Running

```sh
meson test -C build
```

`test-core.c` calls `sw_init()`, which calls `gtk_init()` and needs a display connection even though it needs no full desktop session; `meson.build` runs it under `xvfb-run` when that's available, and directly otherwise.

## Status

Only the `core/` unit tier exists (`test-core.c`), covering the day-zero head's version/error/init surface and the `style/` metrics tokens it depends on. Widget, accessibility, visual-regression, and integration coverage for `SwWidget`/`SwBox`/`SwLabel`/`SwButton`/`SwEntry` — required before any of them clears the [Architecture.md section 4](../../docs/foundational/Architecture.md#4-testing-gate-for-release) release gate — has not been written yet. Tests should be added alongside the code they cover, not written speculatively ahead of it.
