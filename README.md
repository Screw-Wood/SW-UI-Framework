# SW — Screw Wood

**A native C UI toolkit for Linux, with a classic Material-inspired design language.**

SW is a project to make it easier to build polished, consistent desktop applications without asking every application developer to design and implement an entire widget system from scratch.

The project takes inspiration from the visual language of classic Material Design — particularly the Android 8–11 era — and adapts those ideas for desktop applications. SW is not an Android port, a GTK theme, or a libadwaita fork. It is intended to provide its own public component API, interaction conventions, and visual identity.

## Project goals

- Provide reusable, well-behaved desktop widgets through a native C API.
- Give applications a coherent visual language without requiring each project to build its own design system.
- Adapt classic Material ideas to desktop workflows, including mouse, keyboard, window resizing, and accessibility.
- Integrate with the Linux desktop rather than replacing the display server, compositor, or desktop environment.
- Keep the framework modular, documented, and approachable for application developers and contributors.

## Design principles

1. **Native first.** C APIs, shared libraries, normal Linux development tools, and desktop integration.
2. **Consistent by default.** Components should share typography, spacing, color, shape, motion, and interaction rules.
3. **Desktop-aware.** Desktop layouts and input conventions are first-class; mobile patterns are not copied blindly.
4. **Accessible by design.** Keyboard use, focus, screen readers, text scaling, and semantic roles are part of component design.
5. **Composable.** Applications should be able to combine components and build their own higher-level patterns.
6. **Practical dependencies.** Reuse mature platform infrastructure where doing so improves reliability; avoid unnecessary framework layers.
7. **Clear boundaries.** SW owns its component API and design system. It does not attempt to become a desktop environment or application-specific business framework.

## Documentation

Start with the [docs index](docs/README.md). Documentation is organised by where a piece of knowledge sits in time:

- [Foundational](docs/foundational/README.md) — the settled facts: the idea, research, architecture, standards, and design-system values. **Start here if you are a user or new to the project.**
  - [The idea](docs/foundational/IDEA.md) — what SW is, why it exists, who it may help, and what success looks like.
  - [System design](docs/foundational/SYSTEM_DESIGN.md) — architecture, ownership boundaries, platform integration, and development phases.
  - [Research and references](docs/foundational/RESEARCH.md) — existing projects, technical foundations, and primary references.
- [Implementation](docs/Implementation/README.md) — what is being worked on now: build instructions, what the code does today, and the [staged plan](docs/Implementation/STAGES.md).
- [Proposals](docs/proposal/README.md) — planned changes, logged before they are built.
- [Farfuture](docs/Farfuture/README.md) — long-range ideas with no commitment.
- [Bugs](docs/bugs/README.md) — known problems, filed by where they happen.

## Current project definition

The project is in early implementation: the head and a day-zero widget set exist as source, but they have not yet been verified to the release standard, and the toolkit is not production-ready or API-stable. [Implementation](docs/Implementation/README.md) says what exists and what is outstanding. Architecture choices should still be validated with small prototypes before they become permanent commitments.

## Contributing

The most useful contributions are thoughtful discussion, focused prototypes, documentation improvements, accessibility review, and tested code. Before proposing a large subsystem, explain the problem it solves, the boundary it belongs to, and whether an existing platform library can already provide the required capability.

See the repository's contribution and licensing terms before submitting or reusing code. Project governance, contribution workflow, and license should be established explicitly by the maintainers.

---

**SW — Screw Wood.** A toolkit should make good interfaces easier to build, not make application developers fight their tools.
