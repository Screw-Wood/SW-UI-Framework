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

No implementation exists yet. Per [SYSTEM_DESIGN.md](../foundational/SYSTEM_DESIGN.md#16-development-phases), the project is at Phase 0 (validating the premise) moving into Phase 1 (foundation: repository layout, build system, public naming conventions, and the showcase application). This README and the rest of this directory will be filled in as that work lands, rather than in advance of it — implementation docs should describe code that exists, not code that is planned.

See [src/framework/README.md](../../src/framework/README.md) and [src/tests/README.md](../../src/tests/README.md) for the current state of the source tree itself.
