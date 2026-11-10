# Staged implementation plan: finishing day zero

This is the working plan for the in-flight work described in the [Implementation index](README.md). It takes the head and day-zero widget set from [proposal 0003](../proposal/0003-day-zero-widget-set.md) from "source written" to "done to the release standard", in ordered stages.

**Scope.** Exactly what proposal 0003 already owes, and nothing more: widget, accessibility, and visual-regression tests for `SwWidget`/`SwBox`/`SwLabel`/`SwButton`/`SwEntry` ([section 6](../proposal/0003-day-zero-widget-set.md#6-this-does-not-relax-the-release-gate)); the five success criteria in [section 7](../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero) (lifecycle demo, real screen-reader review, checked contrast, and the rest); and a showcase app to host them. New widgets, new features, and anything else belong in [docs/proposal](../proposal/README.md) or [docs/Farfuture](../Farfuture/README.md), not here.

**Maintainer decisions.** Several stages need a call that the proposals and foundational documents do not make. Those are listed per stage under *Decisions needed* and are deliberately left open. Nothing in a stage should be built past one of those points until the maintainer has weighed in, and the answer should be recorded (in the stage, or in a proposal if it changes scope or API).

## Status at a glance

| Stage | Title | Status | Covers |
|---|---|---|---|
| 0 | Scaffolding | Done | Source, Meson build, `core/` unit test |
| 1 | Widget tests | Not started | Architecture.md section 4 (widget tier) |
| 2 | Lifecycle and observer tests | Not started | Success criteria 1, 2 |
| 3 | Accessibility tests | Not started | Success criterion 3; section 4 (accessibility tier) |
| 4 | Contrast checking | Not started | Success criterion 5 |
| 5 | Showcase application | Not started | Hosts criteria 1-4; SYSTEM_DESIGN.md Phase 1 |
| 6 | Visual-regression tests | Not started | Architecture.md section 4 (visual tier) |
| 7 | Screen-reader review | Not started | Success criterion 4 |
| 8 | Day-zero sign-off | Not started | All of the above |

Criteria numbers refer to [proposal 0003 section 7](../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero): (1) full lifecycle in order, (2) `SwEntry` commit-on-`UNFOCUSED`, (3) keyboard-only operation with correct focus order, (4) real screen-reader review, (5) checked contrast.

Stages 1-4 are independent of the showcase and can proceed in that order or in parallel. Stage 5 needs 2 and 3 in good shape so the app demonstrates behaviour that is already tested. Stage 6 needs a decision on rendering determinism (below). Stage 7 needs a running app, so it follows 5. Stage 8 closes everything out.

---

## Stage 0 — Scaffolding (done)

**Result.** Source for the head and the five widget types under `src/framework/`; a Meson build producing `libsw-1.0` and `sw-1.0.pc`; one unit test, `src/tests/unit/test-core.c`, covering `core/` and the metrics tokens.

**Not claimed.** Stage 0 says nothing about whether the widgets or the lifecycle behave correctly. That is what the remaining stages establish.

---

## Stage 1 — Widget tests

**Goal.** A `src/tests/widget/` tier covering, for each of `SwWidget`, `SwBox`, `SwLabel`, `SwButton`, and `SwEntry`, the widget-test items in [SYSTEM_DESIGN.md section 15](../foundational/SYSTEM_DESIGN.md#15-testing-strategy) and [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release).

**Work.**
- Minimum and natural size for each type, including `SwBox` honouring its spacing against the 8dp grid in [design-doc.md section 2](../foundational/design-doc.md#2-spacing-and-layout-grid).
- Resize behaviour: `SwBox` in both orientations, `SwLabel` wrapping/ellipsizing where applicable.
- Pointer interaction for `SwButton`: press, release, and cancel.
- Keyboard interaction: `SwButton` activation; `SwEntry` text entry and editing.
- Interaction states exposed by `SwWidget`: normal, focused, disabled at minimum; `sw_widget_set_disabled` suppresses input.
- Wire the new tier into `src/tests/meson.build`, run under Xvfb like `core`.

**Exit.** Every day-zero type has passing widget tests in the `widget` suite under `meson test`.

**Decisions needed.**
- How input is simulated: GTK's test utilities, synthetic events, or direct activation. This sets how faithful "pointer press/release/cancel" is.
- Whether the base `SwWidget` is tested only through a concrete subclass (it is not directly instantiable).

---

## Stage 2 — Lifecycle and observer tests

**Goal.** Automated evidence for criteria 1 and 2: the state machine from [proposal 0002 section 3](../proposal/0002-lifecycle-ownership-model.md#3-the-state-machine) and the `SwEntry` commit-on-`UNFOCUSED` integration from [section 4](../proposal/0002-lifecycle-ownership-model.md#4-the-observer-interface).

**Work.**
- `SwWindow` reaches `CREATED → SHOWN → FOCUSED` forward and `UNFOCUSED → HIDDEN → DESTROYED` backward, in order.
- `FOCUSED ⇄ UNFOCUSED` and `SHOWN ⇄ HIDDEN` can cycle repeatedly without destroying the window.
- A registered `SwLifecycleObserver` receives every transition, in order, and a window with several observers notifies all of them (the structural chain-up guarantee).
- `sw_window_remove_lifecycle_observer` stops notifications; removing an unregistered observer is a no-op.
- `SwEntry` emits `committed` with its current text on `UNFOCUSED`, and does not emit for programmatic `sw_entry_set_text`.
- `on_destroyed` ordering: observers are not called back into after destruction.

**Exit.** A `lifecycle` suite passes under Xvfb, and each rule from proposal 0002 section 3 maps to at least one test.

**Decisions needed.**
- How window focus changes are driven in a headless environment, since Xvfb has no window manager by default. This may require a lightweight window manager in the test environment or a test hook; it is a test-infrastructure choice the proposals do not make.
- Whether the observer must be removed before finalization is enforced (debug assertion) or only documented, per the open details in [proposal 0002 section 6](../proposal/0002-lifecycle-ownership-model.md#6-open-implementation-details).

---

## Stage 3 — Accessibility tests

**Goal.** The accessibility tier from [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release) and [section 6](../foundational/Architecture.md#6-accessibility-compliance-baseline), automated, plus criterion 3.

**Work.**
- Role, accessible name, and state exposure through `GtkAccessible` for each concrete type, using the contract defined once on `SwWidget` and `sw-accessible`.
- Label relationships where they apply (for example, a `SwLabel` naming a `SwEntry`).
- Keyboard-only operation of `SwButton` and `SwEntry`, with correct focus order across a small composed layout.
- Interactive target sizes against the thresholds in Architecture.md section 6 (pointer: at least 24x24dp).
- Disabled state is announced as disabled.

**Exit.** An `accessibility` suite passes under Xvfb. This stage covers the automated half of criteria 3; the real-assistive-technology half is Stage 7.

**Decisions needed.**
- What "keyboard-only operation" means as a test: scripted key events through a real focus chain, or assertions on focusability and tab order. Scripted events are closer to the criterion but need the same input-simulation decision as Stage 1.

---

## Stage 4 — Contrast checking

**Goal.** Criterion 5: every rendered color/text/UI-component combination meets [design-doc.md section 4](../foundational/design-doc.md#4-color-and-contrast), checked rather than assumed.

**Work.**
- Enumerate every colour role used by the five types in every interactive state (default, hover, focus, pressed; disabled checked for distinguishability, not minimum contrast).
- Compute contrast ratios against the thresholds: normal text 4.5:1, large text 3:1, non-text components (borders, focus indicators) 3:1.
- Make the check a repeatable test so a token change that breaks contrast fails the build, rather than a one-off spreadsheet.
- Record the computed ratios so they can be reviewed.

**Exit.** A contrast test runs under `meson test` and covers every role/state pair the day-zero widgets use. Any failure is either fixed or filed in [docs/bugs/style](../bugs/style/README.md).

**Decisions needed.**
- Which themes exist at day zero. [design-doc.md section 4](../foundational/design-doc.md#4-color-and-contrast) requires each shipped theme to be checked independently; if a dark theme ships, it doubles the matrix here and in Stage 6.
- Whether the check reads token values directly from `style/sw-metrics.h` and the theme, or from rendered output. Reading tokens is cheaper; rendered output is closer to "what the user sees".

---

## Stage 5 — Showcase application

**Goal.** A small application built *only* from `sw_application`, `sw_window`, `SwBox`, `SwLabel`, `SwButton`, and `SwEntry`, as required by [proposal 0003 section 7](../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero). It is the host for the demonstration and for the screen-reader review.

**Work.**
- Create the app (location and build wiring consuming `sw_dep`, per the note in `src/meson.build`).
- Make the lifecycle visible: show the window's state as it changes, so criterion 1 can be observed by a person, not just asserted by a test.
- Include an entry that visibly commits on `UNFOCUSED` (criterion 2).
- Compose a layout that exercises focus order across buttons and an entry (criterion 3).
- Use no widget beyond the five types, so it proves the day-zero set is sufficient.

**Exit.** The app builds with the rest of the tree and runs; each of criteria 1-3 can be shown in it.

**Decisions needed.**
- Where it lives (for example, an `examples/` or `showcase/` directory) and whether it installs.
- Its name and how much it should grow. Any growth beyond the day-zero five widgets is out of scope here and goes through a proposal.

---

## Stage 6 — Visual-regression tests

**Goal.** The visual tier from [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release): reference renderings of the day-zero widgets across the states and conditions in [SYSTEM_DESIGN.md section 15](../foundational/SYSTEM_DESIGN.md#15-testing-strategy).

**Work.**
- Render each type in normal, hover, focus, pressed, and disabled states (and selected where meaningful).
- Cover light and dark themes if both ship, multiple scale factors, and narrow and wide window sizes.
- Store reference images and compare against them, with a way to review and update references deliberately.

**Exit.** A `visual` suite passes, references are checked in or otherwise reproducible, and an intentional visual change requires a reviewed reference update.

**Decisions needed.**
- How rendering is made deterministic enough to compare: fonts, renderer, and scale factor in the test environment. Without this, the tier produces false failures.
- Where reference images are stored, and the tolerance for pixel differences.
- Which scale factors and window sizes are in the day-zero matrix.

---

## Stage 7 — Screen-reader review

**Goal.** Criterion 4: a review with a real screen reader confirming that `SwWidget`'s accessibility contract holds for each concrete type, not just that role and name are present in code.

**Work.**
- Run the showcase app under a real screen reader on the target platform and walk each of the five types.
- Check that each widget is announced with the right role, name, and state, that focus changes are announced, and that disabled controls are reported as disabled.
- Write up the findings, including anything that fails.

**Exit.** A written review exists, naming the screen reader, version, and platform used, with each failure either fixed or filed in [docs/bugs/accessibility](../bugs/accessibility/README.md).

**Decisions needed.**
- Which screen reader(s) and desktop environment count as the reference for "real", and who performs the review. This is a manual step and cannot be settled by tests.

---

## Stage 8 — Day-zero sign-off

**Goal.** Confirm the day-zero scope is complete, and move the resulting facts to where users read them.

**Work.**
- Check all five criteria in [proposal 0003 section 7](../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero) against evidence from Stages 2-7.
- Confirm each of the five types clears the unit, widget, accessibility, and visual tiers in [Architecture.md section 4](../foundational/Architecture.md#4-testing-gate-for-release).
- Write the verified facts into [docs/foundational](../foundational/README.md) (for example, what the head and day-zero widgets guarantee), update the component table in the [Implementation index](README.md), and close out this plan.

**Exit.** The Implementation index shows no outstanding day-zero items, and the corresponding facts live in foundational. This is the point at which the project moves from "documented architecture" to "architecture with evidence it works", as [proposal 0003 section 7](../proposal/0003-day-zero-widget-set.md#7-success-criteria-for-day-zero) puts it.

---

## Updating this document

Change a stage's status in the table and in its section as work lands. If a stage reveals a problem, file it in [docs/bugs](../bugs/README.md) under the area where it occurs and link it from the stage. If the plan needs more than the current scope (more widgets, more tiers), that is a [proposal](../proposal/README.md), not an edit here.
