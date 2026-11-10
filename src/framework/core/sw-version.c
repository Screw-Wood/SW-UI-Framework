/* sw-version.c */

#define SW_COMPILATION
#include "sw-version.h"

const char *
sw_version_string (void)
{
  return SW_VERSION_STRING;
}

gboolean
sw_check_version (guint required_major,
                   guint required_minor,
                   guint required_micro)
{
  if (required_major != SW_VERSION_MAJOR)
    return FALSE;

  if (required_minor > SW_VERSION_MINOR)
    return FALSE;

  if (required_minor == SW_VERSION_MINOR &&
      required_micro > SW_VERSION_MICRO)
    return FALSE;

  return TRUE;
}
