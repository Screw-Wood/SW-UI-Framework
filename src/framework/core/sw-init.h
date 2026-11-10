/* sw-init.h
 *
 * The library entry point. Per docs/proposal/0001-dependency-ownership-policy.md
 * section 3, process/event-loop bring-up mechanics are outsourced to
 * GTK4/GLib; sw_init() is SW's own, owned entry point that wraps that
 * bring-up so application code has a single documented place to start,
 * rather than being required to know it must call gtk_init() itself.
 */

#ifndef SW_INIT_H
#define SW_INIT_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <glib.h>

G_BEGIN_DECLS

/**
 * sw_init:
 * @argc: (inout) (optional): address of the `argc` parameter of main(),
 *   or %NULL
 * @argv: (inout) (array length=argc) (optional): address of the `argv`
 *   parameter of main(), or %NULL
 *
 * Initializes SW. This must be called once, from the main thread, before
 * any other `sw_*` function, and before constructing an #SwApplication
 * or #SwWindow. It is safe to call more than once; subsequent calls are
 * no-ops that return the result of the first call.
 *
 * This also initializes GTK4 (via gtk_init()), per the ownership split
 * in docs/proposal/0001-dependency-ownership-policy.md: platform
 * bring-up is outsourced, but SW owns the single entry point application
 * code calls.
 *
 * Ownership: transfer-none on @argc/@argv; SW does not take ownership of
 * the argument vector.
 *
 * Thread-affinity: main-thread only. Like gtk_init(), this function must
 * be called from the thread that will run the application's main loop.
 *
 * Returns: %TRUE if initialization succeeded.
 */
gboolean sw_init (int    *argc,
                   char ***argv);

/**
 * sw_is_initialized:
 *
 * Checks whether sw_init() has already been called successfully.
 *
 * Thread-affinity: safe to call from any thread.
 *
 * Returns: %TRUE if SW has been initialized.
 */
gboolean sw_is_initialized (void);

G_END_DECLS

#endif /* SW_INIT_H */
