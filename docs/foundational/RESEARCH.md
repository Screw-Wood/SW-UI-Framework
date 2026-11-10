# Research and References

This note records the initial ecosystem and technical research behind SW. It is a starting point, not an exhaustive market survey. Project capabilities and maintenance status can change; revisit external projects before making long-term decisions.

## 1. Existing projects and adjacent solutions

### GTK4

GTK is a general-purpose GUI toolkit with a C-based object-oriented architecture. Its architecture documentation describes several distinct layers:

- **GDK:** window-system abstraction, including Wayland and X11 on Linux/Unix, and other platform backends.
- **GSK:** rendering abstraction that turns drawing operations into render commands.
- **GLib:** core utilities, event loop, threads, dynamic loading, and object system.
- **Pango:** text layout and rendering with internationalization support.
- **GdkPixbuf:** image loading infrastructure.

Why it matters to SW: GTK4 can provide a substantial amount of platform and widget infrastructure without forcing SW to use libadwaita's design system.

Primary source: https://www.gtk.org/docs/architecture/

### libadwaita

libadwaita is a GNOME-oriented library built on GTK. It provides GNOME design conventions and widgets. It is not required merely to use GTK4.

Why it matters to SW: SW's intended design language is distinct from Adwaita. The project should not depend on libadwaita unless a concrete integration need emerges.

Source: https://gnome.pages.gitlab.gnome.org/libadwaita/

### Qt Quick Controls — Material style

Qt Quick Controls includes a Material style based on Google's Material Design guidelines. Its documentation describes theme, primary/accent colors, elevation, and a dense desktop variant.

Why it matters: It demonstrates that a mature Material-styled component set exists, but it is a Qt Quick/QML implementation. SW's stated direction is C and GTK4-based, not Qt.

Source: https://doc.qt.io/qt-6/qtquickcontrols-material.html

### MauiKit

MauiKit is a reusable UI framework for convergent interfaces. Its project documentation describes it as based on Qt Quick Controls 2 and QML, with components intended for Linux and other platforms.

Why it matters: It is an adjacent Linux UI framework, but its technology foundation and design goals differ from SW's proposed C/GTK4 direction.

Sources:
- https://mauikit.org/
- https://github.com/kde/mauikit

### GTK themes

GTK themes can alter appearance, but a theme is not automatically a new widget API, component interaction model, accessibility contract, or developer framework.

Why it matters: SW should be more than CSS. Its value should come from reusable components, consistent behavior, and a documented C API.

## 2. Material Design reference

Google introduced Material Design in 2014 as a unified design language using tactile surfaces, bold graphic design, and meaningful motion. The classic Material guidelines remain published as legacy guidance while Google also presents Material 3 as the newer system.

SW's intended inspiration is the classic Material era associated with Android 8–11. The project should treat those guidelines as a reference and adapt them to desktop ergonomics, not copy mobile dimensions blindly.

Useful references:
- Material Design 2: https://m2.material.io/
- Elevation: https://m2.material.io/design/environment/elevation.html
- Original Material Design announcement: https://developers.googleblog.com/en/this-is-material-design/

The elevation guidance is especially relevant because it explains how surfaces use depth to communicate hierarchy and interaction. SW should define its own desktop-appropriate elevation tokens and avoid treating shadows as decoration without meaning.

## 3. Technical foundations relevant to SW

### GDK and platform integration

GTK documents GDK as the windowing-system abstraction. Its architecture allows GTK to operate across backends such as Wayland and X11.

Implication: SW can target Linux desktop environments without implementing its own Wayland or X11 windowing layer, provided it uses GTK/GDK appropriately.

References:
- https://www.gtk.org/docs/architecture/
- https://docs.gtk.org/gtk4/wayland
- https://docs.gtk.org/gdk4/x11.html

### GTK custom widgets and layout

GTK4 supports custom widget types and layout managers. The GTK documentation explains the widget lifecycle and the role of layout managers in measuring and allocating children.

Implication: SW can implement its own components as GTK custom widget types rather than painting an entire application into a single canvas. This preserves more of GTK's expected lifecycle and integration behavior.

References:
- https://docs.gtk.org/gtk4/class.Widget.html
- https://docs.gtk.org/gtk4/class.LayoutManager.html
- https://blog.gtk.org/2020/04/27/custom-widgets-in-gtk-4-layout/

### Rendering: GSK and snapshots

GTK4's drawing model uses snapshots to build a render-node tree. GSK turns that representation into rendering commands for available renderer backends. Cairo remains available for suitable custom drawing, but it is not necessary to make Cairo the whole rendering architecture.

Implication: Start with GTK's supported rendering path and only add lower-level rendering complexity when a measured requirement justifies it.

References:
- https://docs.gtk.org/gtk4/drawing-model.html
- https://docs.gtk.org/gtk4/class.Snapshot.html
- https://www.cairographics.org/

### Accessibility

GTK's accessibility documentation describes `GtkAccessible` and explains that custom widget authors must provide appropriate roles, properties, relations, and behavior. Accessibility roles communicate a promise to assistive technologies.

Implication: SW's component specifications must include accessible semantics and keyboard behavior. Accessibility should be tested as part of component completion.

Reference:
- https://docs.gtk.org/gtk4/section-accessibility.html

### Text and internationalization

Pango provides text layout and rendering with internationalization support. Text layout is not simply drawing a sequence of glyphs; shaping, scripts, directionality, and font fallback matter.

Implication: SW should use established text infrastructure rather than inventing its own text shaping system.

References:
- https://www.gtk.org/docs/architecture/pango
- https://github.com/GNOME/pango

## 4. Lifecycle models: Android and GTK4

This research backs [proposal 0002](../proposal/0002-lifecycle-ownership-model.md), which formalises SW's application/window lifecycle.

### Android's Activity lifecycle

Android exposes a fixed, OS-enforced sequence of callbacks on `Activity`: `onCreate → onStart → onResume → (running) → onPause → onStop → onDestroy`, with `onRestart` on the way back to the foreground. The framework enforces that overrides call the superclass implementation — omitting it throws `SuperNotCalledException` — so the contract is enforced, not merely documented. `onPause` work is always expected to be undone in `onResume`, since `onResume` is guaranteed to run after a prior `onPause` when an activity returns to the foreground.

On top of that fixed sequence, Jetpack's Architecture Components add a `Lifecycle` / `LifecycleOwner` / `LifecycleObserver` pattern: any component (not just the Activity itself) can observe lifecycle state and react to it, instead of the Activity manually notifying every subsystem it owns.

Implication: the part of Android's DX worth reproducing is not just "there is a lifecycle" — it's that the lifecycle is an enforced contract, and that other components can subscribe to it declaratively rather than being told about it imperatively.

References:
- https://8ksec.io/a-blueprint-of-android-activity-lifecycle/
- https://learn.microsoft.com/en-us/xamarin/android/app-fundamentals/activity-lifecycle/
- https://www.geeksforgeeks.org/?p=576332 (Lifecycle, LifecycleOwner, LifecycleObserver)

### GTK4's application and widget lifecycle

`GApplication`/`GtkApplication` expose a much flatter, signal-based lifecycle: `startup` (once, primary instance only) → `activate`/`open` (may fire multiple times) → `shutdown`. GTK defaults applications to single-instance; a second launch re-signals the first instance's `activate`/`open` and the second process exits without ever calling `startup` or `shutdown`. Separately, `GtkWidget` has its own realize/map/show/hide/unmap/unrealize/destroy sequence, independent of the application-level signals.

Implication: GTK4 already solves the mechanics — process registration, single-instance handling, window/surface binding — that Android's OS layer solves for Activities. It does not provide Android's enforced-contract or observer-pattern ergonomics on top of those mechanics; that layer is what SW needs to add.

References:
- https://docs.gtk.org/gtk4/class.Application.html
- https://developer.gnome.org/documentation/tutorials/application.html
- https://gtk-rs.org/gtk4-rs/git/docs/gtk4/struct.Application.html

## 5. Initial conclusion

The initial research does not establish that no comparable toolkit exists. There are adjacent projects, and Qt already offers a Material style. The plausible niche for SW is narrower:

> A native C toolkit for Linux desktop applications, using GTK4 platform infrastructure, with its own component API and a classic Material-inspired desktop design system, without Qt or libadwaita as dependencies.

That is a hypothesis to validate, not a market claim. The best next step is a prototype that proves the core proposition: a small application can use SW widgets, obtain a coherent Material-inspired appearance, and retain native Linux integration and accessibility.

## 6. Sources consulted

1. GTK architecture — https://www.gtk.org/docs/architecture/
2. GTK4 overview — https://docs.gtk.org/gtk4/overview.html
3. GTK4 widget API — https://docs.gtk.org/gtk4/class.Widget.html
4. GTK4 drawing model — https://docs.gtk.org/gtk4/drawing-model.html
5. GTK4 layout manager — https://docs.gtk.org/gtk4/class.LayoutManager.html
6. GTK4 accessibility — https://docs.gtk.org/gtk4/section-accessibility.html
7. GTK4 Wayland backend — https://docs.gtk.org/gtk4/wayland
8. GDK X11 backend — https://docs.gtk.org/gdk4/x11.html
9. Pango architecture — https://www.gtk.org/docs/architecture/pango
10. Material Design 2 — https://m2.material.io/
11. Material elevation — https://m2.material.io/design/environment/elevation.html
12. Original Material Design announcement — https://developers.googleblog.com/en/this-is-material-design/
13. Qt Quick Controls Material style — https://doc.qt.io/qt-6/qtquickcontrols-material.html
14. MauiKit — https://mauikit.org/
15. MauiKit source — https://github.com/kde/mauikit
16. Android Activity lifecycle (AOSP internals) — https://8ksec.io/a-blueprint-of-android-activity-lifecycle/
17. Android Activity lifecycle overview — https://learn.microsoft.com/en-us/xamarin/android/app-fundamentals/activity-lifecycle/
18. Jetpack Lifecycle/LifecycleOwner/LifecycleObserver — https://www.geeksforgeeks.org/?p=576332
19. GtkApplication class reference — https://docs.gtk.org/gtk4/class.Application.html
20. Using GtkApplication (GNOME tutorial) — https://developer.gnome.org/documentation/tutorials/application.html
21. gtk4-rs Application docs — https://gtk-rs.org/gtk4-rs/git/docs/gtk4/struct.Application.html

Research note prepared for the initial SW project documentation. Re-check external references before using them to justify a new technical or compatibility decision.
