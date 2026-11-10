# SW design-doc: v1 default design system values

**Status:** Accepted. Resolves the token *categories* in [SYSTEM_DESIGN.md §8.2](SYSTEM_DESIGN.md#82-design-tokens) to concrete default numeric values for the v1 corporate MVP (see [Architecture.md](Architecture.md)).

## 1. Purpose

SYSTEM_DESIGN.md §8.2 names the token families SW needs — color, typography, spacing, shape, elevation, component dimensions, motion, focus — without pinning numbers. [IDEA.md §5](IDEA.md#5-design-inspiration-classic-material-adapted-for-desktop) sets the direction: classic Material Design 2 as a reference, adapted for desktop, not copied blindly. This document is where that adaptation gets made concrete: specific default values for v1's shipped theme, with the accessibility floor from [Architecture.md §6](Architecture.md#6-accessibility-compliance-baseline) as a hard constraint on all of them.

These are v1 defaults, not a claim that they're permanently correct. A later proposal can change them; until one does, these are the numbers components are built and tested against.

## 2. Spacing and layout grid

- **Baseline grid: 8dp.** All component spacing, sizing, and layout should align to an 8dp grid, matching classic Material Design's baseline grid guidance, retained because it gives SW the same "mathematical consistency across every screen" Material's own documentation cites as the grid's purpose.
- **Fine grid: 4dp**, for icon alignment and typography detail within a component, per the same Material guidance (icons and type may align to a 4dp sub-grid where the full 8dp grid is too coarse).
- **Interactive target size** (adapted per Architecture.md §6, not copied from mobile unmodified):
  - Pointer/mouse input: minimum 24×24dp clickable target, per Material's desktop/pointer click-target guidance.
  - Touch/gesture input, where SW supports it at all (SYSTEM_DESIGN.md §10 leaves this host-dependent): minimum 48×48dp with at least 8dp between adjacent targets, per Material's touch-target guidance.
- **Minimum screen-edge margin:** 16dp, as a v1 default for top-level window content, consistent with Material's margin guidance; individual components may override this where a container context requires it.

## 3. Elevation scale

Desktop surfaces don't need mobile's full elevation range, but a small, consistent scale is still worth having so "this surface floats above that one" means the same thing everywhere in SW. v1 default elevation levels, adapted from classic Material Design 2's documented component defaults:

| Level | dp | Example SW usage |
|---|---|---|
| 0 | 0 | Flat/text button, standard side sheet (resting) |
| 1 | 1 | Card (resting), switch |
| 2 | 2–8 | Button (default range; exact value per button variant) |
| 4 | 4 | App bar / top bar |
| 6 | 6 | Card (raised/hover), FAB (resting) |
| 12 | 12 | FAB (pressed) |
| 16 | 16 | Navigation drawer, modal bottom/side sheet |
| 24 | 24 | Dialog |

This table is a v1 starting point taken directly from Material 2's own component elevation defaults, not independently re-derived for desktop. If a prototype shows desktop surfaces need a flatter range (per IDEA.md §5's warning against copying Android dimensions blindly), that's a change to make explicitly in a later revision of this document, not silently per-widget.

## 4. Color and contrast

Every official SW theme must meet the thresholds already set as release-gating in [Architecture.md §6](Architecture.md#6-accessibility-compliance-baseline):

- Normal text: ≥ 4.5:1 contrast against its background (WCAG 2.1 SC 1.4.3, AA).
- Large text (≥18pt/24px regular or ≥14pt/~18.67px bold): ≥ 3:1.
- Non-text UI components — borders, focus indicators, meaning-bearing icon parts: ≥ 3:1 against adjacent color (WCAG 2.1 SC 1.4.11, AA).

This applies per color *role* (primary, secondary, surface, outline, on-primary text, status colors), not just to a single default palette instance — any theme SW ships, including a future dark variant, is checked against the same thresholds independently, not assumed to inherit compliance from the light theme's numbers.

Color roles themselves (primary / secondary / surface / background / outline / text / disabled / status) follow the naming already set in SYSTEM_DESIGN.md §8.2; this document does not add new role names, only the contrast constraint every value assigned to them must satisfy.

## 5. Typography

Typography roles (display / headline / title / body / label, per SYSTEM_DESIGN.md §8.2) carry two v1 constraints:

- Every role's default size/weight combination must itself satisfy the contrast thresholds in §4 at whatever size it renders (a `label` role is small enough that WCAG's large-text 3:1 allowance does not apply to it — it needs the full 4.5:1).
- Font stacks must have a real fallback chain (per SYSTEM_DESIGN.md §12) rather than assuming a single font is present; this document does not pin a specific typeface for v1, leaving that to implementation, but the fallback requirement itself is not optional.

## 6. Motion

SYSTEM_DESIGN.md §8.2 lists motion duration, easing, and transition rules as a token family SW needs. This document deliberately does **not** pin specific duration/easing numbers yet — unlike spacing, elevation, and contrast, the research behind this document did not turn up a desktop-adapted motion standard worth committing to, and inventing numbers without that grounding would undermine the rest of this document's sourcing discipline. Motion values are an open item for a follow-up proposal, not a gap silently filled here.

## 7. v1 compliance checklist

A component's visual implementation is not done for v1 until it can be checked off against all of:

- [ ] Spacing and sizing align to the 8dp grid (§2), 4dp sub-grid for icon/type details.
- [ ] Interactive target size meets §2's pointer and, if applicable, touch minimums.
- [ ] Elevation, if the component uses any, matches a level in §3's table (or a documented, deliberate exception).
- [ ] Every color role used, in every theme SW ships, meets §4's contrast thresholds in every interactive state (Architecture.md §6).
- [ ] Every typography role used meets §5's contrast requirement at its actual rendered size.

This checklist is the design-side half of the release gate in [Architecture.md §4](Architecture.md#4-testing-gate-for-release); a component clears both before it ships in a tagged v1 release.

## 8. Primary references

- Material Design spacing methods (8dp grid, touch targets) — https://material.io/design/layout/spacing-methods.html
- Material Design applying density (touch target breakdown) — https://material.io/design/layout/applying-density.html
- Material Design layout guidance (click target minimum, margins) — https://sites.google.com/evaneos.com/guideline/agents-tools/layout
- Material Design component elevation defaults — https://l0neman.github.io/2021/04/21/material-design-参考
- WCAG 2.1 SC 1.4.3 Contrast (Minimum) and SC 1.4.11 Non-text Contrast — https://frontendchecklist.io/rules/accessibility/color-contrast
