# SW — The Idea

## 1. In one paragraph

SW (Screw Wood) is a proposed native UI toolkit for Linux desktop applications, implemented in C and designed around a cohesive, classic Material-inspired visual language. Its purpose is to give application developers a reusable set of components, interaction patterns, design tokens, and integration conventions so they can build attractive, accessible, consistent applications without repeatedly solving the same interface problems. SW takes visual inspiration from the Android 8–11 era of Material Design, but adapts it for desktop workflows rather than copying mobile layouts or attempting to reproduce Android itself.

## 2. What problem is it trying to solve?

Linux has mature GUI infrastructure and several established toolkits. The problem is not that developers cannot draw a button or open a window. The problem is that building a complete, polished application interface involves many interconnected decisions:

- How components look and behave.
- How spacing, typography, colors, shape, and elevation remain consistent.
- How focus, keyboard navigation, hover, pressed, disabled, and selected states work.
- How dialogs, menus, toolbars, navigation, and content layouts fit together.
- How interfaces adapt to different window sizes, display scales, and input methods.
- How accessibility semantics are exposed to assistive technology.
- How applications document and maintain their own component conventions.

When every application solves these problems independently, effort is duplicated and the resulting applications can feel unrelated. A theme can change appearance, but it does not necessarily provide a coherent component API, interaction model, or reusable design system.

SW aims to package those decisions into a reusable framework.

## 3. What SW is

SW is intended to be:

- **A native C library.** Applications should be able to consume it through ordinary headers, shared libraries, and standard Linux build tooling.
- **A widget and component toolkit.** It should provide reusable controls and composable building blocks, not merely a stylesheet.
- **A design system.** It should define consistent tokens and rules for color, typography, spacing, shape, elevation, states, and motion.
- **A desktop framework.** It should respect desktop conventions such as resizable windows, keyboard navigation, context menus, pointer precision, and dense layouts.
- **An integration layer.** It should use established Linux platform infrastructure instead of trying to replace the window system or desktop environment.
- **An open ecosystem.** Its API, documentation, examples, and contribution process should make it possible for other developers to build with and improve it.

## 4. What SW is not

SW is not intended to be:

- A GTK theme or a collection of CSS overrides.
- A libadwaita fork or an attempt to reproduce GNOME's design language.
- A Qt or QML project.
- An Android runtime, Android compatibility layer, or port of Android framework classes.
- A desktop environment, window manager, compositor, display server, or system shell.
- An application-specific framework that dictates how developers store data, access networks, or structure business logic.
- A promise that every Linux application must use the same appearance.
- A reason to reimplement mature infrastructure without a concrete benefit.

The initial technical proposal uses GTK4/GDK as Linux platform infrastructure. That does not mean SW should depend on libadwaita or inherit Adwaita's visual identity.

## 5. Design inspiration: classic Material, adapted for desktop

The intended visual reference is classic Material Design, especially the Android 8–11 period. Relevant ideas include:

- Clear surface hierarchy and restrained elevation.
- Purposeful shadows that communicate layering rather than decoration.
- Strong, legible typography.
- A coherent color system with meaningful primary, secondary, surface, and status colors.
- Familiar controls with clear affordances and states.
- Meaningful, short animations that explain state changes.
- Consistent component geometry and spacing.

The aim is inspiration and adaptation, not a pixel-perfect copy. Desktop interfaces need different density, navigation, window behavior, and input conventions from phone interfaces.

SW should define its own tokens and component specifications. Material Design 2 is a historical design reference and its published guidance remains useful, but SW should not assume that every old guideline is automatically appropriate for a contemporary Linux desktop.

## 6. Who could benefit?

### Application developers

- They can spend more time on their application's purpose and less time rebuilding common controls.
- Shared components reduce the burden of maintaining visual consistency.
- A documented C API provides a familiar integration path for native Linux projects.
- Reusable patterns can make prototypes and production interfaces faster to build.
- A common toolkit can lower the barrier for small teams and solo developers who lack dedicated UI designers.

### End users

- Applications built with SW can offer more consistent interactions and visual behavior.
- Familiar controls can reduce the learning curve between applications.
- Better shared defaults can improve keyboard use, focus behavior, accessibility, and visual clarity.
- A cohesive design language can make independent applications feel more carefully finished without forcing them to look identical.

### The wider Linux ecosystem

- A reusable toolkit can give independent developers another option between adopting an existing design system and maintaining their own.
- Shared components and conventions can encourage collaboration across unrelated applications.
- Improvements to accessibility, input behavior, and component quality can benefit every application using the framework.
- A small, well-documented C library can be useful to projects that prefer native dependencies and straightforward deployment.

These are potential benefits, not guaranteed outcomes. They depend on implementation quality, long-term maintenance, compatibility, accessibility, documentation, and adoption.

## 7. Why not simply use an existing option?

Existing projects solve adjacent problems:

- GTK provides a mature native widget toolkit and platform integration.
- libadwaita provides GNOME-oriented widgets and design conventions on top of GTK.
- Qt Quick Controls includes a Material style, but SW's intended implementation is not Qt/QML.
- MauiKit provides reusable components and convergent application patterns, built on Qt technologies.
- GTK themes can change visual styling, but a theme alone is not a complete alternative component API and design system.

SW should not claim that no alternatives exist. Its possible niche is the combination of a native C developer experience, a distinct classic-Material-inspired desktop design system, and deliberate Linux integration without Qt or libadwaita as application-facing dependencies.

The key test is whether SW provides enough value beyond composing existing GTK components and styles. That must be demonstrated through prototypes and real applications.

## 8. Product principles

### 8.1 Consistency over novelty

A component should feel like part of SW, not like an unrelated widget that happens to share a color.

### 8.2 Desktop first

Use desktop-appropriate control sizes, navigation, keyboard behavior, context menus, and window layouts. Touch support can be considered without making every interface mobile-sized.

### 8.3 Accessibility is functionality

A control is incomplete if it cannot be operated with a keyboard or does not expose meaningful semantics to assistive technology. Accessibility is part of the component contract, not a later polish task.

### 8.4 Composability

The toolkit should offer sensible defaults while allowing application developers to build custom components and layouts without fighting the framework.

### 8.5 Predictability

Public APIs, state transitions, sizing behavior, and theming rules should be documented and testable.

### 8.6 Keep the core focused

The toolkit should not absorb unrelated application concerns. It should integrate with established libraries for platform services and specialized functionality.

### 8.7 Compatibility is a feature

Once a public API is declared stable, changes should be deliberate, documented, and compatible where practical. Early experimentation should be clearly marked as such.

## 9. What a useful first release might contain

The first useful milestone should be a small, coherent vertical slice rather than a large catalogue of unfinished widgets.

A sensible initial set:

- Application/window integration.
- Theme and design-token system.
- Labels and icons.
- Buttons and button groups.
- Text entry and basic form controls.
- Switches, checkboxes, and radio controls.
- Cards and separators.
- Basic containers and layout helpers.
- Dialogs and popovers.
- A small navigation pattern.
- Focus and keyboard behavior.
- Accessibility metadata.
- A showcase application and API documentation.

A control should not be considered complete merely because it renders. It should have defined sizing, states, keyboard behavior, accessibility semantics, scaling behavior, and tests.

v1's target is a corporate MVP: this list should stay small, but everything on it is held to the release-gating standard in [Architecture.md](Architecture.md), not a reduced "good enough for a demo" bar. [Proposal 0003](../proposal/0003-day-zero-widget-set.md) defines the smaller subset of this list — the head plus five widgets — that has to exist first, before the rest of this list is built on top of it.

## 10. What success looks like

SW is succeeding when an independent developer can:

1. Install the development package using normal Linux tooling.
2. Create a small native C application with a documented, understandable API.
3. Compose a useful interface without writing their own widget system.
4. Get consistent visual behavior without manually styling every control.
5. Support keyboard navigation and assistive technology using documented component contracts.
6. Build and distribute the application without depending on undocumented internals.
7. Upgrade the toolkit without repeatedly rewriting application code.

Longer term, success means third-party applications use SW because it makes their work easier and gives users a better experience—not because the toolkit requires them to.

## 11. Risks and honest constraints

- **Scope creep:** A complete UI toolkit is a substantial long-term project. Keep the first release deliberately small.
- **GTK coupling:** Using GTK4 as the initial host platform creates a dependency and influences lifecycle, rendering, and accessibility architecture. Keep SW's public API separate from GTK-specific details where practical.
- **Accessibility complexity:** Custom controls require deliberate semantic and interaction work. Do not postpone this.
- **Visual imitation without behavior:** A Material-like appearance is not enough; component behavior and desktop ergonomics matter just as much.
- **Maintenance burden:** A toolkit needs compatibility policy, tests, documentation, release discipline, and maintainers.
- **Fragmentation:** Another toolkit is only useful if it solves a clear problem better for a defined audience.
- **Platform claims:** Initial Linux support should be stated precisely. Other platforms should only be claimed after real testing and maintained backend support.

## 12. The guiding question

Every proposed feature should answer:

> Does this make it easier for developers to build better native applications, or make those applications clearer, more consistent, more accessible, and more pleasant to use?

If not, it probably does not belong in SW.

---

**SW — Screw Wood.** A framework should take repetitive interface work off developers' hands while leaving their applications recognisably their own.
