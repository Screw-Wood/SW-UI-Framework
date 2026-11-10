# src/meson.build
#
# Builds the SW library described in src/framework/README.md's module
# layout (which follows docs/foundational/SYSTEM_DESIGN.md section 6),
# scoped to what actually exists so far: the core/, application/,
# widgets/, layout/, style/, platform/, and accessibility/ modules
# backing docs/proposal/0003-day-zero-widget-set.md's head and
# day-zero widget set. Modules named in SYSTEM_DESIGN.md section 6
# but not yet implemented (animation/, resources/) are intentionally
# absent here rather than stubbed in, per that section's "add a module
# when it has a real responsibility" guidance.

# sw-version.h is generated from core/sw-version.h.in (docs/foundational/
# Architecture.md section 2: SW follows SemVer, and sw.h's public
# version macros are the compile-time half of that contract). The
# generation itself lives in framework/core/meson.build, not here —
# see that file's header comment for why.
subdir('framework/core')

sw_sources = files(
  'framework/core/sw-init.c',
  'framework/core/sw-version.c',
  'framework/core/sw-error.c',
  'framework/platform/sw-gtk4-bridge.c',
  'framework/accessibility/sw-accessible.c',
  'framework/application/sw-lifecycle.c',
  'framework/application/sw-application.c',
  'framework/application/sw-window.c',
  'framework/widgets/sw-widget.c',
  'framework/widgets/sw-label.c',
  'framework/widgets/sw-button.c',
  'framework/widgets/sw-entry.c',
  'framework/layout/sw-box.c',
)

sw_public_headers = files(
  'framework/sw.h',
  'framework/core/sw-init.h',
  'framework/core/sw-error.h',
  'framework/style/sw-metrics.h',
  'framework/accessibility/sw-accessible.h',
  'framework/application/sw-lifecycle.h',
  'framework/application/sw-application.h',
  'framework/application/sw-window.h',
  'framework/widgets/sw-widget.h',
  'framework/widgets/sw-label.h',
  'framework/widgets/sw-button.h',
  'framework/widgets/sw-entry.h',
  'framework/layout/sw-box.h',
)

# Only the framework/ root is on the include path, plus framework/core:
# every cross-module include in the tree (sw.h's own "module/sw-foo.h"
# includes, and internal sources reaching into a sibling module, e.g.
# application/sw-application.c including "core/sw-init.h") already
# spells out its module prefix, matching what an external consumer's
# single '-I.../sw-1.0' from pkg-config can resolve; a same-module
# include between two hand-written files (e.g. sw-window.c including
# "sw-application.h") resolves via the including file's own directory,
# which the compiler always searches first for quoted includes. The one
# exception is sw-version.c's bare '#include "sw-version.h"': that
# header is generated into the *build* tree (framework/core/meson.build),
# not the source tree, so the including file's own directory doesn't
# contain it there and framework/core needs an explicit entry.
sw_include_dirs = include_directories('framework', 'framework/core')

sw_deps = [glib_dep, gobject_dep, gtk_dep]

sw_lib = library(
  'sw-1.0',
  sw_sources, sw_version_h,
  include_directories: sw_include_dirs,
  dependencies: sw_deps,
  install: true,
)

# framework/sw-version.h.in's generated header, and every other public
# header, is reachable from application code as '<sw.h>' plus whatever
# 'module/sw-foo.h' path sw.h itself uses (see src/framework/sw.h) —
# so the installed layout mirrors the source layout under an SW-specific
# include directory (SYSTEM_DESIGN.md section 14), not a flattened one.
install_headers('framework/sw.h', subdir: 'sw-1.0')
install_headers(
  'framework/core/sw-init.h',
  'framework/core/sw-error.h',
  subdir: 'sw-1.0/core',
)
install_headers(sw_version_h, subdir: 'sw-1.0/core')
install_headers('framework/style/sw-metrics.h', subdir: 'sw-1.0/style')
install_headers('framework/accessibility/sw-accessible.h', subdir: 'sw-1.0/accessibility')
install_headers(
  'framework/application/sw-lifecycle.h',
  'framework/application/sw-application.h',
  'framework/application/sw-window.h',
  subdir: 'sw-1.0/application',
)
install_headers(
  'framework/widgets/sw-widget.h',
  'framework/widgets/sw-label.h',
  'framework/widgets/sw-button.h',
  'framework/widgets/sw-entry.h',
  subdir: 'sw-1.0/widgets',
)
install_headers('framework/layout/sw-box.h', subdir: 'sw-1.0/layout')

# sw_dep is what both src/tests/meson.build and any future
# examples/showcase consume (SYSTEM_DESIGN.md section 14) instead of
# re-deriving include paths and dependencies by hand.
sw_dep = declare_dependency(
  link_with: sw_lib,
  include_directories: sw_include_dirs,
  sources: [sw_version_h],
  dependencies: sw_deps,
)

pkg = import('pkgconfig')
pkg.generate(
  sw_lib,
  name: 'sw-1.0',
  description: 'SW — a native C UI toolkit for Linux with a classic Material-inspired design language',
  subdirs: 'sw-1.0',
)

subdir('tests')
