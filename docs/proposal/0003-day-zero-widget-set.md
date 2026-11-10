# Proposal 0003: The head and the day-zero widget set

**Status:** Accepted. Defines the vertical-slice prototype scope for [SYSTEM_DESIGN.md Phase 0/1](../foundational/SYSTEM_DESIGN.md#16-development-phases), narrower than the full v1 corporate MVP catalogue in [IDEA.md §9](../foundational/IDEA.md#9-what-a-useful-first-release-might-contain).

## 1. Summary

Before SW can claim the lifecycle model (proposal 0002) or the corporate-MVP standard (Architecture.md) actually work, something has to exist that proves it end to end. This proposal defines that minimum: **the head** — `sw_application` and `sw_window`, as specified in proposal 0002 — plus **a day-zero widget set**: the small handful of widgets that converge, independently, across essentially every GUI toolkit's own bootstrap set, because nothing else can be built or tested without them.

This is not the v1 widget catalogue. IDEA.md §9 already lists a broader MVP set (labels/icons, buttons, text entry, switches/checkboxes/radio, cards, dialogs, navigation). This proposal is the subset of that list — plus one thing IDEA.md §9 doesn't name explicitly, the shared widget base — that has to exist *first*, because every other widget in the broader list is built on it.

## 2. Why this specific set

The set below isn't picked from SW's own taste. It's the intersection of what independent, unrelated toolkits each treat as their own bootstrap floor:

- SerenityOS's LibGUI ships, as its documented starting point, an `Application` instance, a `Button`, a `Label`, a `TextBox`/`TextEditor` for single- and multi-line text input, and `VerticalBoxLayout`/`HorizontalBoxLayout` containers — the same five concerns this proposal names, under different names.
- GNOME's own "Create Core Widgets" planning blueprint lists the identical shape: a `Widget`/`Container` core, a `Window` top-level, box-style containers (`HBox`/`VBox`), and `Label`/`Button` among the first control-and-display widgets — with more elaborate controls (checkbox, toggle switch) explicitly marked as later work, not core.
- Nuklear, a minimal immediate-mode C toolkit built specifically for portability and a small codebase, still can't avoid shipping a button, a label, and an edit/text-input primitive as part of its minimal demo surface — even a toolkit optimized to be as small as possible converges on the same set.
- An academic minimal-widget-toolkit assignment (building "S-UITK" from scratch) requires exactly `Label` and `Button` as the first widgets to implement, for the same reason: everything else composes from them.

The pattern across all four, independently arrived at, is: **a window/application head, a base widget contract, a container primitive for layout, static text, a clickable action, and a text-entry field.** That convergence is the evidence base for this proposal's scope — it is not a novel SW opinion about what matters first.

## 3. The head

`sw_application` and `sw_window` are specified in [proposal 0002](0002-lifecycle-ownership-model.md) and are not redefined here. This proposal treats them as a hard prerequisite: no widget work in §4 starts before the head's state machine and observer interface exist, because every widget below is expected to be a lifecycle observer from its first implementation, not retrofitted later (per proposal 0002 §5).

## 4. The day-zero widget set

### `SwWidget` (base, not directly instantiable)

Every widget in SW, starting with the four below, is a concrete type built on a shared base. This base is what proposal 0002's observer interface, Architecture.md's accessibility baseline (§6), and SYSTEM_DESIGN.md's component-state model (§10) attach to. It is listed first and separately because, per the convergent precedent in §2, every toolkit surveyed has some form of this (SerenityOS's implicit `Widget` base beneath `Button`/`Label`; GNOME's blueprint names `Widget` as its own first core item, ahead of any concrete control).

Minimum contract:
- Participates in the lifecycle-observer interface (proposal 0002 §4).
- Exposes the interaction states from SYSTEM_DESIGN.md §10 relevant to it (at minimum: normal, focused, disabled).
- Exposes `GtkAccessible` role/name/state per SYSTEM_DESIGN.md §11 — even a base type with no concrete UI needs this contract defined once, not per-subclass.
- Has a minimum/natural size contract (SYSTEM_DESIGN.md §15 widget tests).

### `SwBox`

A single container primitive: horizontal or vertical stacking, spacing, and alignment, aligned to the design-doc.md §2 8dp grid. This is the only layout primitive in day zero — no grid, no flow layout, no scrolling container yet (SYSTEM_DESIGN.md §9 lists those as later layout requirements). Every toolkit in §2 ships exactly this shape (`HBox`/`VBox` in GNOME's blueprint, `VerticalBoxLayout`/`HorizontalBoxLayout` in LibGUI) before anything more elaborate, because it's the minimum needed to arrange more than one widget on screen at all.

### `SwLabel`

Static text display. No icon support yet (IDEA.md §9 groups "labels and icons" together, but icons are a separate asset-handling concern deferred past day zero). Must satisfy design-doc.md §5's contrast-at-rendered-size requirement from its first implementation, not as a later pass.

### `SwButton`

A single clickable action control. Day zero ships one visual variant, not the full "buttons and button groups" scope from IDEA.md §9 — variant taxonomy (filled/outlined/text) is explicitly an open question from the MVP widget-mapping discussion and is deferred past this proposal. What day zero's `SwButton` must prove is the full interaction contract: pointer press/release/cancel, keyboard activation, focus-visible state meeting design-doc.md's non-text contrast threshold, and correct accessible role/name.

### `SwEntry`

Single-line text input. Included in day zero, not deferred with the rest of "basic form controls" from IDEA.md §9, because it's the one widget in this set that exercises the hardest integration surface early: text editing, IME/input-method behavior (SYSTEM_DESIGN.md §12), keyboard focus semantics distinct from a button's, and — per the lifecycle-observer example named in proposal 0002 §4 — it's the concrete case used there to justify the observer pattern (autosave/commit-on-`UNFOCUSED`). Day zero should prove that specific integration works, not just assert it in a design document. Multi-line/textarea support is explicitly out of scope here, per the open question already flagged in the MVP widget-mapping discussion.

## 5. Explicitly excluded from day zero

Everything else in IDEA.md §9's MVP list — switches, checkboxes, radio controls, cards, separators, dialogs, popovers, the navigation pattern, icons — is real v1 scope, but not day-zero scope. None of it is needed to prove the head and the base widget contract work end to end, and SYSTEM_DESIGN.md §16 already sequences most of it into Phase 2 (form controls, cards) and Phase 3 (dialogs, popovers, navigation). This proposal doesn't change that sequencing; it only makes explicit that day zero is smaller still than "Phase 1."

## 6. This does not relax the release gate

Smallness of scope is not a quality exemption. Per [Architecture.md §4](../foundational/Architecture.md#4-testing-gate-for-release), day zero's five types still owe unit, widget, accessibility, and visual-regression tests, and still owe the design-doc.md §4 contrast thresholds in every interactive state, before they're considered done — a prototype that proves the architecture works is only useful if it proves the architecture works *to the standard that will actually ship*, not a lowered one.

## 7. Success criteria for day zero

Day zero is complete when a small application, built only from `sw_application`, `sw_window`, `SwBox`, `SwLabel`, `SwButton`, and `SwEntry`, demonstrates:

1. The full lifecycle state machine from proposal 0002 (`CREATED → SHOWN → FOCUSED ⇄ UNFOCUSED → HIDDEN → DESTROYED`) firing correctly and in order.
2. At least one real lifecycle-observer integration working, not just specified — `SwEntry` committing its value on `UNFOCUSED`, per proposal 0002 §4's own example.
3. Keyboard-only operation of the whole app, with correct focus order across `SwButton` and `SwEntry`.
4. Real screen-reader review (not just role/name presence in code) confirming `SwWidget`'s accessibility contract holds for each concrete type.
5. Every rendered color/text/UI-component combination meeting design-doc.md §4's contrast thresholds, checked, not assumed.

This is the vertical-slice prototype SYSTEM_DESIGN.md §16 Phase 0 calls for. Meeting it is what moves the project from "documented architecture" to "architecture with evidence it works."

## 8. Related documents

- [Proposal 0002](0002-lifecycle-ownership-model.md) — the head and observer interface this proposal builds on.
- [Architecture.md §4](../foundational/Architecture.md#4-testing-gate-for-release) — the release gate day zero's widgets are held to.
- [design-doc.md](../foundational/design-doc.md) — the spacing/contrast values day zero's widgets must satisfy.
- [SYSTEM_DESIGN.md §16](../foundational/SYSTEM_DESIGN.md#16-development-phases) — the phase sequencing day zero sits ahead of.
- [IDEA.md §9](../foundational/IDEA.md#9-what-a-useful-first-release-might-contain) — the full v1 MVP list day zero is a subset of.

## 9. Primary references

- SerenityOS LibGUI getting-started reference (Application, Button, Label, TextBox/TextEditor, BoxLayout) — https://www.mintlify.com/serenityOS/serenity/api/libgui
- GNOME "Create Core Widgets" blueprint (Widget/Container core, Window, HBox/VBox, Label/Button as first controls) — https://features.launchpad.net/glitter/+spec/create-core-widgets
- Nuklear immediate-mode C GUI toolkit (minimal codebase, button/label/edit as baseline) — https://github.com/liexusong/gui
- S-UITK minimal widget toolkit assignment (Label and Button as first required widgets) — https://apps.hci.rwth-aachen.de/borchers-old/cs377a/handouts/377A-assn3b.pdf
