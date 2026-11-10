# Bugs: application

Bugs that happen in `src/framework/application/`: `SwApplication`, `SwWindow`, and the `SwLifecycleObserver` interface. This includes lifecycle states reached out of order, observers not notified, or notified after destruction, as specified in [proposal 0002](../../proposal/0002-lifecycle-ownership-model.md).

New bugs go here as `NNNN-short-description.md`. See [how to file a bug](../README.md#how-to-file-one).

## Open bugs

- [0001 — self-removing lifecycle observer makes the next observer skip a callback](0001-observer-self-removal-skips-next.md)
- [0002 — window state stays SHOWN, not FOCUSED, after hide and re-show](0002-state-stale-after-hide-reshow.md) (needs re-checking under a real window manager)
