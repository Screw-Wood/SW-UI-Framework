# src/framework

This is where SW's library source will live: the public C API and its implementation, organised by module as proposed in [docs/foundational/SYSTEM_DESIGN.md](../../docs/foundational/SYSTEM_DESIGN.md#6-internal-modules).

## Planned layout

```text
src/framework/
  core/           sw-init, sw-version, sw-error
  application/    sw-application, sw-window
  widgets/        sw-widget, sw-button, sw-label, sw-entry, sw-card, sw-switch, sw-dialog
  layout/         sw-box, sw-grid, sw-center
  style/          sw-theme, sw-color, sw-metrics, sw-typography, sw-elevation
  animation/      sw-animation, sw-transition
  platform/       sw-gtk4-bridge
  accessibility/  sw-accessible
  resources/
```

Module names are illustrative, not a commitment — see SYSTEM_DESIGN.md section 6. Modules should be added when they have a real responsibility, not created ahead of time to match the diagram.

## Conventions

Source in this directory should follow the C API conventions in [SYSTEM_DESIGN.md section 7](../../docs/foundational/SYSTEM_DESIGN.md#7-widget-and-object-model): an `Sw` type prefix and `sw_` function prefix, opaque public structures, documented ownership semantics, and a clear separation between private, experimental, and stable API surfaces.

## Status

No source has been added yet. The project is at Phase 0–1 as described in [SYSTEM_DESIGN.md section 16](../../docs/foundational/SYSTEM_DESIGN.md#16-development-phases): validating the premise with a small prototype before establishing the repository layout and build system in earnest. Once there is real code here, this README should be replaced with a description of what actually exists, and build/usage instructions should move to [docs/Implementation](../../docs/Implementation/README.md).
