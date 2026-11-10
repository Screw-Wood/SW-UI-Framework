/* sw-error.c */

#define SW_COMPILATION
#include "sw-error.h"

GQuark
sw_error_quark (void)
{
  return g_quark_from_static_string ("sw-error-quark");
}
