# Foundation index

These are the documents that establish why SW exists, what it is intended to be, and how it is proposed to be built. They are the starting point for anyone new to the project.

Read them in this order:

1. [IDEA.md](IDEA.md) — the problem SW is trying to solve, what it is and is not, who could benefit, and what success looks like. Start here.
2. [RESEARCH.md](RESEARCH.md) — the ecosystem and technical research that informs the proposal: existing toolkits, design references, platform libraries SW builds on, and the Android/GTK4 lifecycle research behind proposal 0002.
3. [SYSTEM_DESIGN.md](SYSTEM_DESIGN.md) — the proposed architecture: ownership boundaries, the GTK4 integration strategy, module layout, and development phases.
4. [Architecture.md](Architecture.md) — the v1 standard: versioning/ABI policy, coding and review gates, the testing and accessibility release gate, and what "corporate MVP" means for v1 concretely.
5. [design-doc.md](design-doc.md) — the concrete numeric design-system values (spacing grid, elevation scale, contrast thresholds, typography constraints) that SYSTEM_DESIGN.md §8.2's token categories resolve to for v1.

## Where this sits in the lifecycle

Foundational is the **past**: the settled facts and standards of the project, and the directory users and newcomers should read. Per the [docs lifecycle](../README.md#the-lifecycle), content reaches this directory only once it is decided and, for anything that is built, done and verified. Planned work lives in [docs/proposal](../proposal/README.md), work in flight is tracked in [docs/Implementation](../Implementation/README.md), and uncommitted long-range ideas live in [docs/Farfuture](../Farfuture/README.md).

## Status

Nothing here is a claim that the toolkit is production-ready or API-stable. Some documents in this directory (notably [SYSTEM_DESIGN.md](SYSTEM_DESIGN.md)) still carry a *Proposed* status line from when they were written; the architecture they describe is being validated through the day-zero prototype tracked in [Implementation](../Implementation/STAGES.md). What the code does today is documented in Implementation, not here.

Whether to move [SYSTEM_DESIGN.md](SYSTEM_DESIGN.md) from *Proposed* to *Accepted* now that [Architecture.md](Architecture.md) and [design-doc.md](design-doc.md) already build on it as Accepted documents is an open maintainer decision, not yet made; it is tracked alongside the day-zero work in [Implementation](../Implementation/STAGES.md) rather than decided here.

These documents are the stable record of the project's problem statement, rationale, and standards. They are not rewritten to match every implementation detail; when the code and these documents disagree, the disagreement is recorded as a bug in [docs/bugs](../bugs/README.md) or as a new proposal. Accepted proposals ([0001](../proposal/0001-dependency-ownership-policy.md), [0002](../proposal/0002-lifecycle-ownership-model.md), [0003](../proposal/0003-day-zero-widget-set.md)) extend this foundation rather than replacing it.
