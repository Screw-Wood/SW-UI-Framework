/* sw.h
 *
 * The single public entry point for SW. Application code should
 * `#include <sw.h>` and nothing else from SW's tree; individual
 * `sw-*.h` headers guard against being included directly (see each
 * header's `SW_INSIDE_SW_H` check) so that this file stays the one
 * place the public API surface is assembled, per
 * docs/foundational/Architecture.md section 5's documentation standard.
 *
 * Headers below are grouped to match the module layout in this
 * directory's README.md, which in turn follows
 * docs/foundational/SYSTEM_DESIGN.md section 6.
 */

#ifndef SW_H
#define SW_H

#define SW_INSIDE_SW_H

/* core */
#include "core/sw-version.h"
#include "core/sw-init.h"
#include "core/sw-error.h"

/* style */
#include "style/sw-metrics.h"

/* accessibility */
#include "accessibility/sw-accessible.h"

/* application */
#include "application/sw-lifecycle.h"
#include "application/sw-application.h"
#include "application/sw-window.h"

/* widgets (day zero: docs/proposal/0003-day-zero-widget-set.md) */
#include "widgets/sw-widget.h"
#include "widgets/sw-label.h"
#include "widgets/sw-button.h"
#include "widgets/sw-entry.h"

/* layout */
#include "layout/sw-box.h"

#undef SW_INSIDE_SW_H

#endif /* SW_H */
