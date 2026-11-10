# src/tests/meson.build
#
# Day-zero scaffolding test coverage. Per
# docs/foundational/SYSTEM_DESIGN.md section 15, SW's full testing
# strategy has five tiers (unit, widget, accessibility, visual
# regression, integration); only the unit tier for the core/ module
# exists so far. Widget/accessibility/visual coverage for SwWidget,
# SwBox, SwLabel, SwButton, and SwEntry — required before any of them
# clears the docs/foundational/Architecture.md section 4 release gate —
# is tracked as outstanding in docs/Implementation/README.md, not
# silently skipped.

test_core_exe = executable(
  'test-core',
  'unit/test-core.c',
  dependencies: [sw_dep],
)

# sw_init() calls gtk_init(), which requires a display connection even
# though it needs no full desktop session (SYSTEM_DESIGN.md section 15).
# Run under Xvfb when available so the unit tier stays runnable in a
# headless CI environment; fall back to running directly if a display
# is already available and Xvfb is not installed.
xvfb_run = find_program('xvfb-run', required: false)

if xvfb_run.found()
  test(
    'core',
    xvfb_run,
    args: ['--auto-servernum', test_core_exe],
    suite: 'unit',
  )
else
  test(
    'core',
    test_core_exe,
    suite: 'unit',
  )
endif
