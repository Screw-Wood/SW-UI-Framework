/* sw-metrics.h
 *
 * Spacing/sizing design tokens. Values are the v1 defaults accepted in
 * docs/foundational/design-doc.md section 2; this header is where that
 * document's numbers become something widget code actually links
 * against, instead of being hand-copied as magic numbers per widget.
 *
 * This is a starting point for the "style" module described in
 * src/framework/README.md, scoped to what the day-zero widget set
 * (docs/proposal/0003-day-zero-widget-set.md) needs: the spacing grid
 * and interactive target minimums. Color, typography, elevation, and
 * motion tokens (SYSTEM_DESIGN.md section 8.2) are further scaffolding
 * work, not yet represented here.
 */

#ifndef SW_METRICS_H
#define SW_METRICS_H

#if !defined(SW_INSIDE_SW_H) && !defined(SW_COMPILATION)
#error "Only <sw.h> can be included directly."
#endif

#include <glib.h>

G_BEGIN_DECLS

/* Baseline spacing grid, in device-independent pixels (dp).
 * design-doc.md section 2: "Baseline grid: 8dp." All SW component
 * spacing, sizing, and layout should align to this. */
#define SW_METRICS_GRID_BASELINE_DP (8)

/* Fine grid for icon alignment and in-component typography detail.
 * design-doc.md section 2: "Fine grid: 4dp." */
#define SW_METRICS_GRID_FINE_DP (4)

/* Minimum clickable target size for pointer/mouse input.
 * design-doc.md section 2, adapted from Material's desktop/pointer
 * click-target guidance per Architecture.md section 6. */
#define SW_METRICS_TARGET_POINTER_MIN_DP (24)

/* Minimum touch/gesture target size and minimum spacing between
 * adjacent touch targets, where SW supports touch input at all
 * (host-dependent per SYSTEM_DESIGN.md section 10). */
#define SW_METRICS_TARGET_TOUCH_MIN_DP (48)
#define SW_METRICS_TARGET_TOUCH_SPACING_MIN_DP (8)

/* Minimum screen-edge margin for top-level window content.
 * design-doc.md section 2. Individual components may override this
 * where a container context requires it. */
#define SW_METRICS_SCREEN_EDGE_MARGIN_DP (16)

G_END_DECLS

#endif /* SW_METRICS_H */
