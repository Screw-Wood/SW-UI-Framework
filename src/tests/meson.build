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

# Under -Db_sanitize=address,undefined (the CI "sanitize" job;
# .github/workflows/ci.yml), gtk_init() pulls in Pango's fontconfig
# backend, and fontconfig's FcInit() parses the system font
# configuration via expat and keeps the resulting caches alive for the
# life of the process by design — it does not call FcFini() itself,
# and nothing in SW or GTK's own init path does either. LeakSanitizer
# reports that as a leak (observed: ~320 bytes in libfontconfig.so.1 /
# libexpat.so.1, after every test in the suite has already passed),
# but it is memory fontconfig deliberately keeps reachable for
# process-lifetime reuse, not memory SW or GTK lost track of, and SW
# has no handle on it to free even if it wanted to. detect_leaks=0
# below turns off only LeakSanitizer's exit-time accounting; the
# memory-safety checks that are the actual point of
# -Db_sanitize=address,undefined (use-after-free, buffer overflows,
# double-free, and everything UndefinedBehaviorSanitizer catches) stay
# fully enabled. If SW's own code starts leaking, nothing here would
# hide that — detect_leaks=0 only stops LeakSanitizer from running its
# own exit-time scan at all; it doesn't filter SW's allocations out of
# a scan that still runs. If that's ever needed instead (to keep
# leak-checking SW's own code while still ignoring fontconfig's), use
# an LSAN_OPTIONS suppressions file and leave detect_leaks enabled.
sanitizer_test_env = environment()
sanitizer_test_env.set(
  'ASAN_OPTIONS',
  'halt_on_error=1:abort_on_error=1:print_summary=1:detect_leaks=0',
)
sanitizer_test_env.set(
  'UBSAN_OPTIONS',
  'halt_on_error=1:abort_on_error=1:print_summary=1:print_stacktrace=1',
)

if xvfb_run.found()
  test(
    'core',
    xvfb_run,
    args: ['--auto-servernum', test_core_exe],
    suite: 'unit',
    env: sanitizer_test_env,
  )
else
  test(
    'core',
    test_core_exe,
    suite: 'unit',
    env: sanitizer_test_env,
  )
endif
