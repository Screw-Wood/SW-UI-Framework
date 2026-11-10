# Foundation index

These are the documents that establish why SW exists, what it is intended to be, and how it is proposed to be built. They are the starting point for anyone new to the project.

Read them in this order:

1. [IDEA.md](IDEA.md) — the problem SW is trying to solve, what it is and is not, who could benefit, and what success looks like. Start here.
2. [RESEARCH.md](RESEARCH.md) — the ecosystem and technical research that informs the proposal: existing toolkits, design references, platform libraries SW builds on, and the Android/GTK4 lifecycle research behind proposal 0002.
3. [SYSTEM_DESIGN.md](SYSTEM_DESIGN.md) — the proposed architecture: ownership boundaries, the GTK4 integration strategy, module layout, and development phases.
4. [Architecture.md](Architecture.md) — the v1 standard: versioning/ABI policy, coding and review gates, the testing and accessibility release gate, and what "corporate MVP" means for v1 concretely.
5. [design-doc.md](design-doc.md) — the concrete numeric design-system values (spacing grid, elevation scale, contrast thresholds, typography constraints) that SYSTEM_DESIGN.md §8.2's token categories resolve to for v1.

## Status

These documents describe a proposed project and a proposed architecture. Nothing here is a claim that the toolkit is implemented, production-ready, or API-stable. Decisions marked as proposed should be validated with small prototypes before they are treated as permanent.

As the project moves from proposal to implementation, [docs/proposal](../proposal/README.md) records specific decisions — including [0001](../proposal/0001-dependency-ownership-policy.md), the dependency ownership policy, and [0002](../proposal/0002-lifecycle-ownership-model.md), the lifecycle ownership model — and [docs/Implementation](../Implementation/README.md) will track the actual state of the code. These foundational documents should stay stable as a record of the original problem statement and rationale; they are not meant to be rewritten to match every implementation detail. Accepted proposals extend this foundation rather than replacing it.
