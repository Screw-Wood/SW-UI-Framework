# Implementation index

This directory documents SW as it is actually built, as opposed to [docs/foundational](../foundational/README.md), which documents SW as proposed. Content here should describe real code in `src/`: module layout, build instructions, public API reference, and component status.

## Purpose

As components are implemented, this is where their documentation should live:

- Build and development setup (toolchain, Meson usage, dependencies).
- Module-by-module description of `src/framework`, matching the layout proposed in SYSTEM_DESIGN.md section 6.
- Public API reference, generated or maintained alongside the source.
- Component status: which widgets exist, which design-system tokens are implemented, and which items from the [initial component set](../foundational/IDEA.md#9-what-a-useful-first-release-might-contain) are complete versus outstanding.
- Notes on where the implementation has diverged from the original proposal, and why.

## Status

Per [SYSTEM_DESIGN.md](../foundational/SYSTEM_DESIGN.md#16-development-phases), the project has moved from Phase 0 (validating the premise) into early Phase 1 (foundation): the repository layout and Meson build system now exist, and build against the head and day-zero widget set from [proposal 0003](../proposal/0003-day-zero-widget-set.md) — `sw_application`, `sw_window`, `SwWidget`, `SwBox`, `SwLabel`, `SwButton`, `SwEntry`.

### Build and development setup

Dependencies: a C toolchain, Meson (`>= 0.60`), GTK4 (`>= 4.10`) and GLib/GObject (`>= 2.70`) development packages.

```sh
meson setup build
meson compile -C build
meson test -C build       # runs under xvfb-run if available
meson install -C build    # installs headers under $includedir/sw-1.0 and a sw-1.0.pc
```

### What's real versus what's outstanding

- Builds clean under `-Wall -Wextra`, with a working `sw-1.0.pc` consumers can build against via plain `pkg-config --cflags --libs sw-1.0` (verified against a real install prefix, not just in-tree).
- Only `core/`'s unit tests exist (`src/tests/test-core.c`). Proposal 0003 section 7's day-zero success criteria — the full lifecycle firing in order, a real `SwEntry` commit-on-`UNFOCUSED` observer integration, keyboard-only operation across `SwButton`/`SwEntry`, a real screen-reader review, and checked (not assumed) contrast — have not been demonstrated yet, and none of the day-zero widgets have widget/accessibility/visual-regression coverage. None of this is release-gated-complete per [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release).
- No showcase application yet (SYSTEM_DESIGN.md Phase 1).
- No generated public API reference yet; the public headers under `src/framework/` are documented inline per [Architecture.md section 5](../foundational/Architecture.md#5-documentation-standard).

See [src/framework/README.md](../../src/framework/README.md) and [src/tests/README.md](../../src/tests/README.md) for the current state of the source tree itself.
