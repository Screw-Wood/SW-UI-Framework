# meson.build (top level)
#
# Build-system scaffolding for SW (docs/foundational/SYSTEM_DESIGN.md
# section 14: Meson for configuration/builds, pkg-config for consumer
# discovery). This file only sets up the project and hands off to
# src/meson.build; it deliberately does not hard-code a minimum GTK4
# version or dependency pin beyond what SYSTEM_DESIGN.md section 14
# already allows ("selected after a working prototype and a
# support-policy decision") — the version constraints below are the
# day-zero prototype's own requirements, not a published support
# matrix (that is still open per SYSTEM_DESIGN.md section 18).

project(
  'sw', 'c',
  version: '0.1.0',
  license: 'TBD',
  meson_version: '>= 0.60.0',
  default_options: [
    'c_std=gnu11',
    'warning_level=2',
  ],
)

sw_version_parts = meson.project_version().split('.')
sw_version_major = sw_version_parts[0]
sw_version_minor = sw_version_parts[1]
sw_version_micro = sw_version_parts[2]

# Dependencies. Per docs/proposal/0001-dependency-ownership-policy.md
# section 3, window-system/event-loop/text-shaping/rendering mechanics
# are outsourced to GTK4 and its own dependencies (GDK, GLib, Pango);
# SW links against them directly rather than vendoring or wrapping them.
glib_dep = dependency('glib-2.0', version: '>= 2.70')
gobject_dep = dependency('gobject-2.0', version: '>= 2.70')
gtk_dep = dependency('gtk4', version: '>= 4.10')

subdir('src')
