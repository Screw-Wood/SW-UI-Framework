/* sw-error.h
 *
 * SW's shared GError domain. Component-specific error codes should be
 * added to SwError as they are needed, rather than each module defining
 * its own GQuark domain, so application code has one error domain to
 * check for anything originating in SW itself (as opposed to errors
 * GTK4/GLib raise directly, which keep their own domains).
 */

#ifndef SW_ERROR_H
#define SW_ERROR_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <glib.h>

G_BEGIN_DECLS

/**
 * SwError:
 * @SW_ERROR_NOT_INITIALIZED: an `sw_*` call was made before sw_init()
 *   succeeded.
 * @SW_ERROR_INVALID_STATE: an operation was attempted while the
 *   relevant #SwWindow was in a lifecycle state that does not permit it
 *   (see docs/proposal/0002-lifecycle-ownership-model.md).
 * @SW_ERROR_FAILED: a generic, otherwise-unclassified failure.
 *
 * Error codes in SW's shared #GError domain. This enum is append-only
 * under SW's compatibility policy (docs/foundational/Architecture.md
 * section 2): existing members keep their numeric value across a stable
 * `MAJOR.MINOR` series.
 */
typedef enum
{
  SW_ERROR_NOT_INITIALIZED,
  SW_ERROR_INVALID_STATE,
  SW_ERROR_FAILED,
} SwError;

/**
 * sw_error_quark:
 *
 * Gets the #GQuark for SW's shared error domain.
 *
 * Thread-affinity: safe to call from any thread.
 *
 * Returns: the error domain quark.
 */
GQuark sw_error_quark (void);

#define SW_ERROR (sw_error_quark ())

G_END_DECLS

#endif /* SW_ERROR_H */
