# Proposal 0001: Dependency ownership policy

**Status:** Accepted.

## 1. Summary

SW's purpose is to bring Android-style application-developer DX and lifecycle discipline to Linux desktop apps: it exists to absorb the non-business-logic boilerplate an app would otherwise have to hand-roll — window/app lifecycle plumbing, consistent component state and behavior, accessibility wiring, and the design-system decisions that have nothing to do with what the application actually does.

GTK4 is SW's current platform dependency, chosen because it already solves window-system integration, rendering, text shaping, and accessibility plumbing well enough that SW does not need to rebuild them to deliver on that purpose (see [SYSTEM_DESIGN.md §3](../foundational/SYSTEM_DESIGN.md#3-ownership-boundaries) and [§19](../foundational/SYSTEM_DESIGN.md#19-architecture-decision-record-initial-gtk4-foundation)). GTK4 is not SW's identity. It is the current answer to a question this proposal makes a standing, repeatable policy rather than a one-time architectural choice: **for any given concern, does SW own it, or does SW outsource it to a dependency?**

## 2. The policy

For any concern a new subsystem, widget, or feature touches, ask:

1. **Is this concern part of the Android-DX promise?** That is: does owning it directly reduce boilerplate an app developer would otherwise write themselves, in a way that is specific to what SW is for (consistent components, enforced lifecycle, accessible-by-default behavior, a coherent design system)? If yes, SW owns it, regardless of whether a dependency could technically provide a version of it. Ownership here is what the product *is*.
2. **Is this concern platform mechanics that a mature dependency already solves reliably?** Window-system binding, event-loop plumbing, text shaping, image decoding, and similar low-level infrastructure fall here. If a dependency solves it well and SW gains nothing product-relevant by reimplementing it, outsource it. Reimplementing mature infrastructure without a concrete, demonstrated benefit is scope creep, not differentiation (see [SYSTEM_DESIGN.md §17](../foundational/SYSTEM_DESIGN.md#17-key-risks-and-mitigations), "Rebuilding GTK accidentally").
3. **Re-evaluate the outsourced concerns as the project and the ecosystem change.** An answer of "outsource to GTK4" today is not permanent. If a dependency's limitations start constraining the DX promise in (1) — or a concern currently outsourced turns out to need SW-specific behavior to deliver that promise — the concern moves from outsourced to owned, or SW evaluates a different dependency for it. This review should happen per-concern, at the point a concern becomes a real constraint, not on a fixed schedule and not as a wholesale re-architecture.

In short: **own anything that is the product. Outsource anything that is infrastructure the product sits on — and keep checking, concern by concern, as circumstances change.** GTK4 is the current outsourcing choice for platform infrastructure; it is a product decision, not a foundational commitment, and future concerns may be outsourced elsewhere, or brought back in-house, as the same questions are asked again.

## 3. Current application of the policy

| Concern | Decision | Rationale |
|---|---|---|
| Window-system binding (Wayland/X11), event loop, process/single-instance handling | Outsourced to GTK4/GDK/GLib | Mature infrastructure; no product-specific reason to reimplement ([SYSTEM_DESIGN.md §3.2](../foundational/SYSTEM_DESIGN.md#32-sw-delegates-to-gtk4-and-its-supporting-libraries)) |
| Text shaping, internationalized layout | Outsourced to Pango (via GTK) | Same as above |
| Rendering pipeline | Outsourced to GTK4's snapshot/GSK path initially | Same as above; revisit only if a measured requirement justifies lower-level control ([SYSTEM_DESIGN.md §8.1](../foundational/SYSTEM_DESIGN.md#81-rendering)) |
| Application/window lifecycle *contract* (enforced states, observer pattern for subsystems) | Owned by SW | This is the core DX promise this project exists to deliver — see [proposal 0002](0002-lifecycle-ownership-model.md) |
| Component states, keyboard/focus behavior, accessibility semantics | Owned by SW | Boilerplate every app would otherwise reimplement per-widget; central to the product, not infrastructure ([SYSTEM_DESIGN.md §3.1](../foundational/SYSTEM_DESIGN.md#31-sw-owns)) |
| Design tokens and visual system | Owned by SW | Same as above |

This table should be extended as new concerns come up, rather than re-litigating the general question each time.

## 4. Consequences

- Every new proposal that introduces a subsystem should state explicitly, using this policy's two questions, whether SW owns or outsources the concern, instead of assuming GTK4 covers it by default.
- GTK4 remains documented as a real, named dependency (per [SYSTEM_DESIGN.md §3.2](../foundational/SYSTEM_DESIGN.md#32-sw-delegates-to-gtk4-and-its-supporting-libraries)), not an implementation detail SW can quietly swap without consequence — swapping or supplementing it is a real migration, and this policy does not make that free, only deliberate.
- "Set in stone" here means the *policy* is settled, not that every individual row in §3's table is permanent. Rows change as concerns are re-evaluated per §2.3; the policy for how to re-evaluate them does not.

## 5. Related documents

- [SYSTEM_DESIGN.md §3](../foundational/SYSTEM_DESIGN.md#3-ownership-boundaries) — the ownership boundaries this policy formalizes a process around.
- [Proposal 0002](0002-lifecycle-ownership-model.md) — the first concrete application of this policy, applied to the application/window lifecycle.
