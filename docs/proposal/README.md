# Proposal index

This directory is for formal, reviewed proposals: concrete decisions about SW's API, architecture, or scope that go beyond the general direction set out in [docs/foundational](../foundational/README.md).

## Purpose

The foundational documents describe SW's overall intent and a proposed architecture. They are deliberately high-level and are not meant to capture every detail or to be revisited for each individual design decision. This directory is where those individual decisions should live once they need to be made explicitly — for example:

- A specific component's public API before it is implemented.
- A change to an architectural decision recorded in [SYSTEM_DESIGN.md](../foundational/SYSTEM_DESIGN.md).
- A resolution to one of the open decisions listed in SYSTEM_DESIGN.md section 18 (license, minimum GTK4 version, ABI policy, theme customization surface, and similar).
- A new subsystem or dependency that was not part of the original proposal.

## What a proposal should contain

A proposal does not need a fixed template yet, but it should generally cover:

- The problem being solved and why it matters.
- The proposed approach, and any alternatives considered.
- Which ownership boundary it falls under (see SYSTEM_DESIGN.md section 3).
- Compatibility or migration impact, if any.
- Open questions that still need resolution.

## Status

No proposals have been written yet — the project is still at the foundational stage described in [docs/foundational](../foundational/README.md). This README will be expanded with a template and an index of accepted proposals once the first one is drafted.
