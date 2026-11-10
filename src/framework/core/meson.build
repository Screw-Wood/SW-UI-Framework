# src/framework/core/meson.build
#
# Generates core/sw-version.h from sw-version.h.in (docs/foundational/
# Architecture.md section 2's SemVer macros — see sw-version.h.in's own
# header comment). This lives in a meson.build of its own, rather than
# being inlined into src/meson.build with an explicit 'core/' output
# path, because Meson's configure_file() does not accept a path segment
# in 'output': generating it from a meson.build that already lives in
# core/ is what makes the build-tree location match the
# '#include "core/sw-version.h"' path the rest of the tree (sw.h, and
# every public header's "Only <sw.h> can be included directly" guard)
# already uses.
sw_version_h = configure_file(
  input: 'sw-version.h.in',
  output: 'sw-version.h',
  configuration: {
    'SW_VERSION_MAJOR': sw_version_major,
    'SW_VERSION_MINOR': sw_version_minor,
    'SW_VERSION_MICRO': sw_version_micro,
    'SW_VERSION_STRING': meson.project_version(),
  },
)
