# 0001: sw-1.0.pc lists GTK/GLib/GObject as Requires.private, breaking plain pkg-config consumers

**Status:** Open
**Found:** 2026-11-10, HEAD at the time of the "Docs update" commit (built and installed locally with GTK 4.14.5 / Meson, Ubuntu packages)
**Affects:** `src/meson.build` (the `pkg.generate()` call), the installed `sw-1.0.pc`

## What happens

The generated `sw-1.0.pc` lists GTK4, GLib, and GObject under `Requires.private`, not `Requires`. A consumer program that calls `pkg-config --cflags --libs sw-1.0` and then uses any GTK/GLib/GObject symbol directly — which any real SW application does, since SW's public headers return and accept GTK types (`GtkWidget *`, etc.) — fails to link: `Requires.private` entries are only pulled into a *static* link's flags, not a plain dynamic `--cflags --libs` query.

## What you expected

[Implementation/README.md](../../Implementation/README.md) claims a consumer can "build against it with `pkg-config --cflags --libs sw-1.0`" without qualification. That claim only held for a program using `sw_*` symbols alone and not touching GTK/GLib/GObject directly, which is not a realistic SW consumer given the public API's own types.

## How to reproduce

1. `meson setup build && meson compile -C build && meson install -C build` (with an appropriate prefix).
2. Write a minimal program that calls `sw_init()` and a GTK function directly (e.g. to run a `GMainLoop`).
3. Compile it with `cc $(pkg-config --cflags --libs sw-1.0) prog.c -o prog`.
4. Observed: link failure, undefined references to GTK/GLib/GObject symbols. Expected: link succeeds.

## Notes

In `src/meson.build`, `pkg.generate(sw_lib, ...)` is called with no `libraries:`/`requires:` override, so Meson's pkgconfig module falls back to treating `sw_lib`'s own `dependencies:` (`sw_deps = [glib_dep, gobject_dep, gtk_dep]`) as private. Since those types are exposed through SW's *public* headers rather than only used internally, they need to be public (`Requires`), which `pkg.generate()` supports via its `requires:` keyword. No fix has been made to `src/meson.build`; the README.md claim about plain `pkg-config --cflags --libs` has not yet been corrected to reflect this either.
