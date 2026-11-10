# SW Architecture: the v1 standard

**Status:** Accepted. Sets the engineering bar for v1; extends [SYSTEM_DESIGN.md](SYSTEM_DESIGN.md), does not replace it.

## 1. Purpose

[SYSTEM_DESIGN.md](SYSTEM_DESIGN.md) says *what* SW is built from. [IDEA.md §9](IDEA.md#9-what-a-useful-first-release-might-contain) says roughly *which* widgets v1 should contain. Neither says *how good* v1 has to be before it counts as done. This document is that bar.

**v1's target is a corporate MVP, not a hobbyist proof-of-concept.** Concretely: v1 should be something an organization can depend on for a real internal or shipped application — not just a demo that renders Material-looking widgets on a showcase screen. That changes what "MVP" is allowed to mean. A corporate MVP is smaller in scope than a mature toolkit, but nothing inside that smaller scope gets a pass on stability, accessibility, documentation, or support. Cutting scope is fine; cutting quality inside the scope that ships is not.

This single decision cascades into concrete, checkable standards below. Every one of them is a gate a component or release must pass, not aspirational guidance.

## 2. Versioning and compatibility policy

SW's public API follows [Semantic Versioning 2.0.0](https://semver.org/): given `MAJOR.MINOR.PATCH`, MAJOR increments for incompatible API changes, MINOR for backward-compatible additions, and PATCH for backward-compatible bug fixes. SemVer's own precondition applies directly: software using it must declare a public API, in code or in documentation, precisely and comprehensively — which is why §5 below makes full public-API documentation a merge gate, not a nice-to-have.

C libraries carry an ABI as well as an API, and the two don't break on the same schedule. SW adopts the parallel-installability pattern GTK itself uses for exactly this reason: GTK's parallel installability guarantees mean GTK2 and GTK3 reverse dependencies can be phased out over time instead of forcing a flag day, because a major version bump gets its own package/soname rather than silently replacing the old one in place. Applied to SW:

- The ABI is frozen within a stable `MAJOR.MINOR` series, following the same pattern D-Bus documents for its own C library: any ABI found in a stable release is frozen, and ABI is not added within a stable series if it can be helped — the ABI of a `1.2.0` and a later `1.2.5` should match, while a `1.4.x` series may add more.
- An ABI-breaking `MAJOR` bump ships as a parallel-installable package (e.g. `libsw-1.0` alongside a future `libsw-2.0`), mirroring GTK3/GTK4 coexistence, so existing corporate consumers are never silently broken by an in-place upgrade.
- Deprecate before removing: a symbol marked deprecated in a `MINOR` release stays functional until the next `MAJOR`.
- Experimental API (per [SYSTEM_DESIGN.md §7.3](SYSTEM_DESIGN.md#73-public-api-stability)) is explicitly exempt from all of the above and may change in any release — but it must be labeled as such in the public header and docs, not left ambiguous.

## 3. Coding and review standards

- Every change to public `sw_*` API requires review before merging to the main branch; no direct pushes to public-API surface.
- Ownership semantics (borrowed / transfer-none / transfer-full, per [SYSTEM_DESIGN.md §7.1](SYSTEM_DESIGN.md#71-c-api-conventions)) must be stated for every function that returns or accepts a pointer — this is a review-blocking requirement, not a documentation nice-to-have, because it's exactly the class of bug a corporate consumer can't safely work around from outside the library.
- CI must run with AddressSanitizer and UndefinedBehaviorSanitizer (or equivalent) on at least the primary supported platform target; a release does not ship with known sanitizer failures.
- Main-thread-only functions (per SYSTEM_DESIGN.md §7.1) must be documented as such; nothing in the public API may silently assume a thread context without saying so.

## 4. Testing gate for release

[SYSTEM_DESIGN.md §15](SYSTEM_DESIGN.md#15-testing-strategy) already defines the test categories SW needs: unit, widget, accessibility, visual regression, integration. This document promotes that list from a strategy to a **release gate**: a component is not "v1-complete" and a version is not tagged for release unless, for every widget included in that release:

- Unit tests pass for its token/metric/state-transition logic.
- Widget tests cover minimum/natural size, resize behavior, and pointer/keyboard interaction.
- Accessibility tests cover role, name, state exposure, and keyboard-only operation (see §6).
- Visual regression tests cover light/dark themes (if both ship) and the interaction states listed in §6.

A widget that renders but fails any of these is not part of the corporate MVP. It can exist on a branch; it does not ship in a tagged release.

## 5. Documentation standard

Every public `sw_` symbol must be documented before it merges: purpose, parameters, ownership transfer, thread-affinity, and any lifecycle-state dependency (see proposal 0002). A corporate consumer must be able to use the public API correctly from documentation alone, without reading SW's source — that is the actual test this standard is checked against, not "is there a doc comment present."

## 6. Accessibility compliance baseline

SW adopts WCAG 2.1 Level AA's numeric thresholds as the hard minimum for every officially shipped SW theme, adapted from web to desktop rendering. These are not goals to work toward — a theme that doesn't meet them does not ship:

| Criterion | Threshold | Source |
|---|---|---|
| Normal text contrast | ≥ 4.5:1 against its background | WCAG 2.1 SC 1.4.3, Level AA |
| Large text contrast (≥18pt/24px regular, or ≥14pt/~18.67px bold) | ≥ 3:1 | WCAG 2.1 SC 1.4.3, Level AA |
| Non-text UI components (borders, focus indicators, meaning-bearing icon parts) | ≥ 3:1 against adjacent color | WCAG 2.1 SC 1.4.11, Level AA |
| Interactive target size (touch/gesture input active) | ≥ 48×48dp, ≥ 8dp between adjacent targets | Material Design spacing guidance |
| Interactive target size (pointer/mouse input) | ≥ 24×24dp | Material Design layout guidance, click target minimum |

Contrast must be checked across every interactive state SW defines (default, hover, focus, pressed) per [SYSTEM_DESIGN.md §10](SYSTEM_DESIGN.md#10-input-focus-and-interaction); disabled state is conventionally exempt from the contrast minimum but must still be visually distinguishable from enabled. Keyboard-only operation and correct `GtkAccessible` role/name/state exposure (SYSTEM_DESIGN.md §11) are release-gating per §4 above, not optional polish.

## 7. Security and supply chain

- SW declares a minimum supported GLib/GTK4/Pango version range per release and does not knowingly ship against a version of those with an unpatched known vulnerability at release time.
- Dependency versions are pinned in the build, not left floating, so a release is reproducible.
- Any SW-owned parser or input-handling code (not mere pass-through to GTK) gets fuzz-tested before it's considered release-gated.

## 8. Support and release engineering

- Each `MAJOR` version gets a stated support window, published alongside the release.
- Every release carries a changelog entry; silent behavior changes within a `MINOR`/`PATCH` are a defect, not an acceptable cost of shipping.
- Deprecation always precedes removal by at least one full `MAJOR` version cycle (see §2).

## 9. How this relates to other documents

- [SYSTEM_DESIGN.md](SYSTEM_DESIGN.md) defines what SW is built from (modules, ownership boundaries, GTK4 integration). This document defines the bar anything built from that architecture has to clear before it counts as part of v1.
- [design-doc.md](design-doc.md) gives the concrete numeric values (spacing, elevation, contrast, type scale) that §6 above and SYSTEM_DESIGN.md §8.2's token categories resolve to for v1's default theme.
- [Proposal 0001](../proposal/0001-dependency-ownership-policy.md) and [0002](../proposal/0002-lifecycle-ownership-model.md) remain the record of *why* specific ownership and lifecycle decisions were made; this document does not revisit them, only holds whatever ships to the standard above.

## 10. Primary references

- Semantic Versioning 2.0.0 — https://semver.org/
- D-Bus API/ABI stability policy (comparable C library precedent) — https://github.com/aosp-mirror/platform_external_dbus/blob/main/README
- GTK3/GTK4 parallel installability discussion — https://lwn.net/Articles/831867
- WCAG 2.1 SC 1.4.3 Contrast (Minimum) and SC 1.4.11 Non-text Contrast — https://frontendchecklist.io/rules/accessibility/color-contrast
- Material Design touch target and spacing guidance — https://material.io/design/layout/spacing-methods.html
- Material Design click target guidance — https://sites.google.com/evaneos.com/guideline/agents-tools/layout
