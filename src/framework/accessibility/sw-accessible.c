/* sw-accessible.c */

#define SW_COMPILATION
#include "sw-accessible.h"

gboolean
sw_accessible_has_role (GtkAccessible *accessible)
{
  GtkAccessibleRole role;

  g_return_val_if_fail (GTK_IS_ACCESSIBLE (accessible), FALSE);

  role = gtk_accessible_get_accessible_role (accessible);

  return role != GTK_ACCESSIBLE_ROLE_NONE &&
         role != GTK_ACCESSIBLE_ROLE_GENERIC;
}
