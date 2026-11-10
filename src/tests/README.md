# src/tests

This is where tests for `src/framework` will live, following the testing strategy proposed in [docs/foundational/SYSTEM_DESIGN.md section 15](../../docs/foundational/SYSTEM_DESIGN.md#15-testing-strategy).

## Planned layout

```text
src/tests/
  unit/           token parsing/lookup, color and metric utilities, state transitions, lifecycle helpers
  widget/         minimum/natural size, layout and resize behavior, pointer/keyboard interaction, focus traversal
  accessibility/  accessible role and name, state exposure, label relationships, keyboard-only operation
  visual/         light/dark themes, scale factors, interaction states, window sizes
  integration/    Wayland/X11 sessions, packaging, shutdown and object lifecycle, input methods and clipboard
```

A component should not be considered complete merely because it renders — see SYSTEM_DESIGN.md section 15 and [IDEA.md section 9](../../docs/foundational/IDEA.md#9-what-a-useful-first-release-might-contain). Sizing, states, keyboard behavior, accessibility semantics, and scaling behavior each need test coverage before a widget is treated as done.

## Status

No tests exist yet, because no implementation exists yet (see [src/framework/README.md](../framework/README.md)). Tests should be added alongside the code they cover, not written speculatively ahead of it.
