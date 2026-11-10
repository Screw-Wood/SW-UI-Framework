# SW System Design

**Status:** Proposed architecture; validate through prototypes before treating it as a permanent contract.

## 1. Purpose

This document defines the proposed responsibilities, boundaries, architecture, and development strategy for SW (Screw Wood), a native C UI toolkit for Linux desktop applications with a classic Material-inspired design system.

It is deliberately explicit about what SW owns and what it delegates to existing platform infrastructure. The central design goal is to build a real reusable toolkit without accidentally turning the project into a window manager, display server, operating system, or duplicate of every library in the GTK stack.

## 2. Architectural decisions at a glance

| Area | Proposed direction |
|---|---|
| Primary language | C |
| Initial platform target | Linux desktop |
| Initial platform integration | GTK4/GDK |
| Widget implementation | SW-defined components implemented as GTK4 custom widgets where appropriate |
| Rendering | GTK4 snapshot/GSK pipeline initially |
| Text shaping/layout | Pango through GTK integration |
| Core object model | GObject, subject to API prototype |
| Build system | Meson |
| Package discovery | pkg-config |
| Design reference | Classic Material Design, adapted for desktop |
| libadwaita | Not a dependency |
| Qt/QML | Not used |
| Public API | C-first, with future bindings possible |
| Compatibility policy | Establish before declaring the API stable |

This is a proposal, not a claim that these choices have all been validated.

## 3. Ownership boundaries

A clean boundary is essential. SW should own the user-facing component system and visual rules, while relying on proven infrastructure for operating-system integration.

### 3.1 SW owns

- Public `sw_*` C API and component contracts.
- SW widget types and composition patterns.
- SW design tokens: color, typography roles, spacing, shape, elevation, and motion.
- Component states: normal, hover, pressed, focused, selected, disabled, loading, and other relevant states.
- Component-specific keyboard and pointer interaction behavior.
- SW-level layout helpers and reusable patterns.
- Theme selection and application-level theme configuration.
- Component documentation, examples, and visual regression tests.
- Compatibility policy for SW's public API.

### 3.2 SW delegates to GTK4 and its supporting libraries

- Top-level native windows and surfaces.
- Window-system integration and backend selection through GDK.
- Event loop and core utility infrastructure through GLib.
- Rendering infrastructure through GTK's snapshot/GSK pipeline.
- Text shaping and internationalized text layout through Pango.
- Image decoding through established image libraries where appropriate.
- Platform input integration, including pointer, keyboard, and input-method pathways exposed by GTK.
- Clipboard, drag-and-drop, and other platform facilities through GTK/GDK APIs where suitable.

GTK documents GDK as its window-system abstraction for backends including Wayland and X11, and GSK as its rendering abstraction. GLib supplies core runtime facilities, while Pango handles text layout. Reusing these avoids making SW responsible for a display server or a complete rendering stack.

### 3.3 SW does not own

- A Wayland compositor or X11 server.
- A window manager, desktop shell, panel, launcher, or notification daemon.
- System-wide settings storage or desktop preference services.
- Application business logic, databases, networking, or domain models.
- A browser engine, media framework, or general-purpose application runtime.
- A replacement for the Linux accessibility bus.
- A guarantee that every desktop environment will adopt SW's visual style.
- A promise of Windows or macOS support until those targets are explicitly designed, implemented, tested, and maintained.

## 4. Proposed layered architecture

```text
┌──────────────────────────────────────────────────────────┐
│                    Applications                           │
│          ARK projects · third-party C applications        │
└──────────────────────────────┬───────────────────────────┘
                               │ public C API
┌──────────────────────────────▼───────────────────────────┐
│                    SW public API                          │
│      sw_application · sw_window · sw_button · ...         │
└──────────────────────────────┬───────────────────────────┘
                               │
┌──────────────────────────────▼───────────────────────────┐
│               SW components and behaviors                 │
│    state · focus · interaction · composition · layout     │
└──────────────────────────────┬───────────────────────────┘
                               │
┌──────────────────────────────▼───────────────────────────┐
│                    SW design system                       │
│       tokens · themes · typography · shape · motion       │
└──────────────────────────────┬───────────────────────────┘
                               │
┌──────────────────────────────▼───────────────────────────┐
│                GTK4 integration layer                      │
│      GtkWidget · GtkSnapshot/GSK · GDK · GLib · Pango      │
└──────────────────────────────┬───────────────────────────┘
                               │
┌──────────────────────────────▼───────────────────────────┐
│           Linux desktop / window-system backends          │
│                 Wayland · X11 (via GDK)                    │
└──────────────────────────────────────────────────────────┘
```

The diagram represents responsibility, not necessarily separate shared libraries. The initial implementation can be a single library with internal modules, provided its internal boundaries remain understandable.

## 5. GTK4 integration strategy

### 5.1 Why GTK4 is a reasonable initial foundation

GTK4 is not libadwaita. GTK4 is a general-purpose widget toolkit and platform layer. SW can use GTK4 without adopting GNOME's design language or depending on libadwaita.

GTK's architecture offers useful building blocks:

- GDK abstracts window-system interaction and supports multiple backends.
- GTK provides widget lifecycle, focus, event controllers, layout, and accessibility interfaces.
- GSK represents rendering work as a scene graph and provides renderer backends.
- GLib supplies the main loop, data structures, object system, and other runtime utilities.
- Pango provides internationalized text layout and shaping.

This is a practical way to get native Linux application integration without writing a display server, windowing backend, text engine, and renderer from scratch.

### 5.2 Recommended widget strategy

Prefer SW components implemented as GTK custom widget types (`GtkWidget` subclasses) where that gives SW control over appearance and behavior while preserving GTK's platform integration.

This approach means:

- SW owns the public component names, API, visual specification, and component behavior.
- GTK owns the underlying widget lifecycle and the platform facilities it already provides.
- SW widgets render through GTK's supported snapshot/GSK path.
- SW widgets must correctly expose accessibility roles, properties, states, and relations.
- SW must use GTK's focus, event, and input systems correctly rather than implementing a parallel event loop.

Do not begin by rendering an entire application into one giant drawing area. That shortcut makes hit-testing, focus, text editing, accessibility, selection, input methods, and embedded content much harder. A custom root surface may be useful for a specialised canvas, but it should not be the default architecture for a general-purpose widget toolkit.

### 5.3 Avoid leaking GTK types into every public API

GTK types will necessarily appear at some integration points if SW widgets subclass GTK widgets. Still, avoid exposing backend-specific details unnecessarily.

A practical public API can:

- Use SW-prefixed types for SW-owned controls.
- Document when a function accepts or returns a GTK type.
- Avoid exposing private rendering structures.
- Keep theme tokens and component behavior independent of Adwaita CSS classes.
- Avoid making undocumented GTK internals part of SW's ABI.

Do not build a speculative abstraction layer around every GTK function. Abstract only where SW needs a stable contract or a real alternative implementation.

### 5.4 Future backend portability

The first supported environment should be Linux with GTK4. GDK already abstracts several window systems, including Wayland and X11. This provides useful portability within Linux environments.

A future non-GTK backend is a separate architectural project. It would require equivalents for windowing, input, accessibility, rendering, text, clipboard, drag-and-drop, and packaging. Do not advertise a backend as supported until it has tests and maintainers.

## 6. Internal modules

The initial codebase can be organised into modules such as:

```text
src/
  core/
    sw-init.c
    sw-version.c
    sw-error.c

  application/
    sw-application.c
    sw-window.c

  widgets/
    sw-widget.c
    sw-button.c
    sw-label.c
    sw-entry.c
    sw-card.c
    sw-switch.c
    sw-dialog.c

  layout/
    sw-box.c
    sw-grid.c
    sw-center.c

  style/
    sw-theme.c
    sw-color.c
    sw-metrics.c
    sw-typography.c
    sw-elevation.c

  animation/
    sw-animation.c
    sw-transition.c

  platform/
    sw-gtk4-bridge.c

  accessibility/
    sw-accessible.c

  resources/
    ...

tests/
  unit/
  integration/
  accessibility/
  visual/

examples/
  showcase/
```

Names are illustrative. Do not create empty modules solely to match this tree; add a module when it has a real responsibility.

## 7. Widget and object model

### 7.1 C API conventions

- Use a consistent `Sw` type prefix and `sw_` function prefix.
- Prefer opaque public instance structures and private implementation data.
- Document ownership transfer and reference-counting expectations for every returned object.
- Use `GError` for recoverable failures where appropriate.
- Keep constructors, setters, getters, signals, and properties predictable.
- Avoid exposing private struct fields as ABI.
- Document whether functions are main-thread-only.
- Make ownership rules explicit: borrowed, transfer-none, or transfer-full.

### 7.2 GObject

GObject is a strong candidate because GTK's widget model is already based on it. It provides runtime types, properties, signals, and reference counting that fit C libraries.

Use it because it solves concrete object-model needs, not because every internal helper must become a GObject. Plain C structs are appropriate for private data and short-lived value types.

### 7.3 Public API stability

Separate the API into three categories:

1. **Private:** implementation details; no compatibility promise.
2. **Experimental:** available for testing, explicitly allowed to change.
3. **Stable:** documented and covered by compatibility policy.

Do not declare the API stable until core object lifecycle, widget ownership, event behavior, and theme semantics have been exercised by real applications.

## 8. Rendering and visual system

### 8.1 Rendering

Use GTK4's snapshot/GSK pipeline initially. A custom widget's snapshot implementation should describe its content through supported GTK rendering APIs. GSK then handles renderer-specific execution.

Cairo can be used for suitable 2D drawing where appropriate, but it should not be assumed to be the universal primary renderer. Avoid selecting a GPU API prematurely.

### 8.2 Design tokens

The design system should be token-driven rather than hard-coded independently in every widget.

Token families should include:

- Color roles: primary, secondary, surface, background, outline, text, disabled, and semantic status colors.
- Typography roles: display, headline, title, body, label, and supporting text.
- Spacing scale.
- Shape and corner-radius scale.
- Elevation levels and shadow parameters.
- Component dimensions and density.
- Motion duration, easing, and transition rules.
- Focus indicators and interaction-state overlays.

Token names should describe intent rather than encode a specific color value. For example, a widget should use a semantic primary-action token rather than hard-code a particular blue.

### 8.3 Classic Material inspiration

Use classic Material principles as a reference:

- Surface hierarchy and elevation communicate relationships.
- Motion communicates state changes and spatial relationships.
- Color supports hierarchy and action recognition.
- Components have consistent geometry and predictable interaction states.

Do not mechanically copy Android pixel dimensions. Desktop density, window size, pointer input, keyboard interaction, and user scaling require deliberate adaptation.

### 8.4 Theme behavior

The toolkit should provide a default SW theme and a documented way for applications to select supported variants. Customization should be intentional and bounded.

Avoid promising arbitrary CSS compatibility unless the project deliberately implements and maintains such a styling contract. A token API and component-level style properties may be easier to keep stable.

## 9. Layout and responsive behavior

The toolkit should support composition and layouts that respond to available space.

Initial layout requirements:

- Horizontal and vertical composition.
- Spacing and alignment.
- Minimum and natural sizing.
- Expand/fill behavior.
- Grid or flow layout for appropriate content.
- Scrollable content.
- Sensible behavior when windows become narrow.
- Display scaling and text scaling.

Prefer GTK's layout facilities initially where they satisfy the need. Add SW-specific layout managers only when SW needs behavior that GTK's existing managers cannot express cleanly.

## 10. Input, focus, and interaction

Every interactive component needs a defined state machine.

At minimum, consider:

- Pointer enter/leave.
- Press, release, cancellation, and activation.
- Keyboard activation.
- Focus and focus-visible behavior.
- Disabled state.
- Selected, checked, expanded, and indeterminate states where applicable.
- Touch or gesture behavior where supported by the host.
- High-contrast and reduced-motion preferences where available.

Use GTK event controllers and focus infrastructure. Do not create a competing global event loop.

Keyboard behavior should be documented for each component. A button should activate using expected keyboard input; text controls must preserve text editing conventions; composite widgets must define focus movement.

## 11. Accessibility

Accessibility is a core subsystem, not optional polish.

For every interactive widget, define:

- Correct accessible role.
- Accessible name and description.
- State and value exposure.
- Relationships to labels, descriptions, and controls.
- Keyboard operability.
- Focus order and visible focus.
- Announcements for state changes where required.
- Behavior with screen readers and other assistive technology.

GTK's `GtkAccessible` interface exposes semantics to assistive technology. GTK documentation notes that custom widget authors must supply appropriate accessibility information and behavior. A role is a behavioral promise: declaring a widget to be a button means it should behave like a button.

Accessibility tests should be part of component acceptance, not a future milestone.

## 12. Text, internationalization, and icons

- Use Pango/GTK text facilities rather than hand-positioning glyphs.
- Support Unicode and right-to-left layouts where GTK provides the relevant facilities.
- Do not assume a fixed font is installed. Define a preferred typography stack with robust fallback.
- Ensure labels can be translated and do not assume fixed string lengths.
- Use scalable icon assets where practical.
- Document icon naming, sizing, alignment, and accessible labels.
- Do not use icons as the only accessible name for an action.

## 13. Threading and lifecycle

The initial design should assume UI object mutation occurs on the GTK/GLib main context unless a specific API documents otherwise.

- Background work should not block the UI thread.
- Document how applications marshal results back to the UI context.
- Widget destruction and callback disconnection must be safe and predictable.
- Avoid hidden global mutable state.
- Theme changes should invalidate only the necessary rendering and layout state.
- Animation callbacks must stop when their target objects are no longer valid.

## 14. Build, packaging, and developer experience

Proposed tools:

- Meson for configuration and builds.
- pkg-config metadata for consumer projects.
- Public headers installed under an SW-specific include directory.
- A shared library with deliberate symbol visibility.
- Tests runnable without requiring a full desktop session where possible.
- A small showcase application.
- API documentation generated from source comments.

The exact dependency list and minimum GTK version should be selected after a working prototype and a support-policy decision. Do not hard-code a version in these design documents until maintainers choose one.

An eventual application should be able to consume SW using ordinary C build tooling, for example through a pkg-config package name chosen by the project.

## 15. Testing strategy

### Unit tests
- Token parsing and lookup.
- Color and metric utilities.
- State transitions.
- Ownership and lifecycle helpers.

### Widget tests
- Minimum/natural size.
- Layout and resize behavior.
- Pointer and keyboard interactions.
- Focus traversal.
- Disabled and selected states.

### Accessibility tests
- Accessible role and name.
- State exposure.
- Label relationships.
- Keyboard-only operation.
- Screen-reader review with real assistive technology.

### Visual regression tests
- Light and dark themes if both are supported.
- Normal and dense desktop sizing.
- Multiple scale factors.
- Hover, focus, pressed, disabled, and selected states.
- Narrow and wide window sizes.

### Integration tests
- Wayland and X11 sessions where available.
- Packaging and downstream consumer builds.
- Application shutdown and object lifecycle.
- Input methods, clipboard, and drag-and-drop where relevant.

## 16. Development phases

### Phase 0 — Validate the premise
- Compare existing GTK-based alternatives and themes.
- Build a tiny GTK4 custom-widget prototype.
- Verify the intended visual direction with a showcase mock-up.
- Confirm accessibility and focus integration are feasible.
- Decide licensing and contribution governance.

### Phase 1 — Foundation
- Establish repository layout and build system.
- Define public naming and object conventions.
- Create application/window integration.
- Implement theme tokens and basic rendering helpers.
- Create the showcase application.

### Phase 2 — First coherent component set
- Implement labels, buttons, containers, cards, and basic form controls.
- Add focus and keyboard behavior.
- Add accessibility semantics and tests.
- Document the public API.

### Phase 3 — Application patterns
- Add dialogs, popovers, navigation patterns, toolbars, and menus.
- Improve responsive behavior and density.
- Add visual regression and integration tests.

### Phase 4 — Stabilisation
- Test with at least one application beyond the showcase.
- Define ABI/API compatibility rules.
- Document packaging and upgrade expectations.
- Publish a supported dependency and platform matrix.

### Phase 5 — Ecosystem
- Consider language bindings and contributor-maintained extensions.
- Evaluate additional platform backends only with clear demand and maintainers.
- Maintain design guidelines, component examples, and migration notes.

## 17. Key risks and mitigations

| Risk | Mitigation |
|---|---|
| Rebuilding GTK accidentally | Delegate platform services and lifecycle to GTK; implement only SW's differentiated layer |
| One giant custom drawing surface | Use real widget types for controls and text; reserve canvas rendering for canvas-like components |
| Poor accessibility | Make accessibility part of widget acceptance criteria |
| GTK coupling leaks everywhere | Keep SW naming, tokens, and public contracts distinct; document unavoidable GTK integration points |
| Inconsistent components | Define component specs and shared tokens before expanding the widget catalogue |
| API churn | Mark experimental APIs clearly; stabilise only after real application use |
| Overpromised portability | State the supported platform matrix honestly and test every claimed backend |
| Unbounded scope | Deliver a small complete component set before adding breadth |

## 18. Open decisions

These decisions should be made through prototypes or explicit project policy:

- Project license.
- Minimum GTK4 version and supported Linux distributions.
- Exact GObject conventions and public ABI policy.
- Theme customization surface.
- Whether UI definitions/builders are in scope.
- Icon source and licensing.
- Internationalization workflow.
- Release cadence and support window.
- Contribution governance and code review policy.
- Whether language bindings are generated through GObject Introspection or maintained separately.

## 19. Architecture decision record: initial GTK4 foundation

**Decision:** Use GTK4/GDK as the initial Linux platform foundation, while SW owns its component API and design system. Do not depend on libadwaita or Qt.

**Why:** GTK4 already supplies mature Linux window-system integration, widget lifecycle, rendering infrastructure, event handling, and accessibility integration. Reusing it reduces the amount of low-level infrastructure SW must implement before it can deliver useful components.

**Consequences:**
- SW is a GTK4-based native toolkit in its initial architecture, not a completely independent windowing/rendering stack.
- SW's public components and visual identity remain its own.
- Custom widgets must correctly integrate with GTK lifecycle, focus, rendering, and accessibility contracts.
- GTK4 remains a real dependency and should be documented as such.
- A future non-GTK backend is not free; it requires a deliberate compatibility layer and substantial implementation work.

**Status:** Proposed; validate with a vertical-slice prototype before locking the design.

## 20. Primary technical references

- GTK architecture: https://www.gtk.org/docs/architecture/
- GTK4 overview: https://docs.gtk.org/gtk4/overview.html
- GTK4 widget API: https://docs.gtk.org/gtk4/class.Widget.html
- GTK4 drawing model: https://docs.gtk.org/gtk4/drawing-model.html
- GTK4 layout manager: https://docs.gtk.org/gtk4/class.LayoutManager.html
- GTK4 accessibility: https://docs.gtk.org/gtk4/section-accessibility.html
- GTK4 Wayland: https://docs.gtk.org/gtk4/wayland
- Pango architecture: https://www.gtk.org/docs/architecture/pango
- Material Design 2: https://m2.material.io/
- Material elevation: https://m2.material.io/design/environment/elevation.html
