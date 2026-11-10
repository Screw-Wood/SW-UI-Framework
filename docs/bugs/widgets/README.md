# Bugs: widgets

Bugs that happen in `src/framework/widgets/`: `SwWidget` (the base), `SwLabel`, `SwButton`, and `SwEntry`. This includes interaction-state bugs, `SwEntry` commit behaviour, and anything the widgets owe under [proposal 0003](../../proposal/0003-day-zero-widget-set.md).

New bugs go here as `NNNN-short-description.md`. See [how to file a bug](../README.md#how-to-file-one).

## Open bugs

- [0001 — SwButton keyboard activation does nothing, and warns on every key press](0001-button-keyboard-activation-noop.md) (blocks day-zero criterion 3)
- [0002 — SwButton emits clicked when released outside the button](0002-button-clicked-outside-release.md)
- [0003 — each SwEntry is two Tab stops, and the outer one accepts no typing](0003-entry-double-tab-stop.md) (blocks day-zero criterion 3)
- [0004 — nothing enforces the 24dp pointer target minimum; SW_METRICS_* tokens are unused](0004-pointer-target-minimum-unenforced.md)
- [0005 — SwEntry::committed also fires on the entry's own focus-out](0005-entry-committed-fires-on-own-focus-out.md)
