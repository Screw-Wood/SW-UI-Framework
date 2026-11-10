/* test-core.c
 *
 * Unit tests for the core/ module: sw_init(), sw_version_string(),
 * sw_check_version(), and sw_error_quark(). Deliberately does not
 * construct an SwWindow or any widget. sw_init() itself calls
 * gtk_init(), which still needs *a* display connection even though it
 * needs no full desktop session; src/tests/meson.build runs this test
 * under Xvfb for exactly that reason, per
 * docs/foundational/SYSTEM_DESIGN.md section 15's "Tests runnable
 * without requiring a full desktop session where possible". Widget/
 * accessibility/visual tests for the day-zero widgets
 * (docs/proposal/0003-day-zero-widget-set.md section 6's release gate)
 * are not yet present; see docs/Implementation/README.md for what is
 * still outstanding.
 */

#include <sw.h>

static void
test_version_string_matches_macros (void)
{
  const char *version = sw_version_string ();
  g_autofree char *expected =
    g_strdup_printf ("%d.%d.%d", SW_VERSION_MAJOR, SW_VERSION_MINOR, SW_VERSION_MICRO);

  g_assert_cmpstr (version, ==, expected);
}

static void
test_check_version_same_major (void)
{
  g_assert_true (sw_check_version (SW_VERSION_MAJOR, SW_VERSION_MINOR, SW_VERSION_MICRO));
  g_assert_true (sw_check_version (SW_VERSION_MAJOR, 0, 0));
  g_assert_false (sw_check_version (SW_VERSION_MAJOR, SW_VERSION_MINOR + 1, 0));
}

static void
test_check_version_different_major (void)
{
  /* Per docs/foundational/Architecture.md section 2's parallel-install
   * model, a MAJOR mismatch is never compatible, regardless of
   * minor/micro. */
  g_assert_false (sw_check_version (SW_VERSION_MAJOR + 1, 0, 0));
}

static void
test_error_quark_is_stable (void)
{
  g_assert_cmpuint (sw_error_quark (), ==, sw_error_quark ());
  g_assert_cmpuint (SW_ERROR, ==, sw_error_quark ());
}

static void
test_metrics_grid_tokens (void)
{
  /* docs/foundational/design-doc.md section 2: the fine grid divides
   * the baseline grid evenly, and the pointer target minimum is smaller
   * than the touch target minimum. These are sanity checks on the
   * accepted numbers, not a re-derivation of them. */
  g_assert_cmpint (SW_METRICS_GRID_BASELINE_DP % SW_METRICS_GRID_FINE_DP, ==, 0);
  g_assert_cmpint (SW_METRICS_TARGET_POINTER_MIN_DP, <, SW_METRICS_TARGET_TOUCH_MIN_DP);
}

static void
test_init_is_idempotent (void)
{
  g_assert_true (sw_is_initialized ());
  g_assert_true (sw_init (NULL, NULL));
  g_assert_true (sw_is_initialized ());
}

int
main (int   argc,
      char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_assert_true (sw_init (NULL, NULL));

  g_test_add_func ("/core/version-string-matches-macros", test_version_string_matches_macros);
  g_test_add_func ("/core/check-version-same-major", test_check_version_same_major);
  g_test_add_func ("/core/check-version-different-major", test_check_version_different_major);
  g_test_add_func ("/core/error-quark-is-stable", test_error_quark_is_stable);
  g_test_add_func ("/core/metrics-grid-tokens", test_metrics_grid_tokens);
  g_test_add_func ("/core/init-is-idempotent", test_init_is_idempotent);

  return g_test_run ();
}
