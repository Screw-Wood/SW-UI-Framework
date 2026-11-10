/* sw-box.c */

#define SW_COMPILATION
#include "sw-box.h"

struct _SwBox
{
  SwWidget parent_instance;
};

G_DEFINE_TYPE (SwBox, sw_box, SW_TYPE_WIDGET)

static void
sw_box_dispose (GObject *object)
{
  GtkWidget *child;

  while ((child = gtk_widget_get_first_child (GTK_WIDGET (object))) != NULL)
    gtk_widget_unparent (child);

  G_OBJECT_CLASS (sw_box_parent_class)->dispose (object);
}

static void
sw_box_class_init (SwBoxClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = sw_box_dispose;

  /* GtkBoxLayout already implements exactly the day-zero contract
   * (docs/proposal/0003-day-zero-widget-set.md section 4: "horizontal
   * or vertical stacking, spacing, and alignment"), so SwBox reuses it
   * per the outsourcing side of
   * docs/proposal/0001-dependency-ownership-policy.md rather than
   * reimplementing box layout math SW gains nothing product-specific
   * from owning. */
  gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BOX_LAYOUT);
  gtk_widget_class_set_accessible_role (widget_class, GTK_ACCESSIBLE_ROLE_GROUP);
  gtk_widget_class_set_css_name (widget_class, "sw-box");
}

static void
sw_box_init (SwBox *self)
{
  (void) self;
}

GtkWidget *
sw_box_new (GtkOrientation orientation,
            int            spacing)
{
  GtkWidget *box = g_object_new (SW_TYPE_BOX, NULL);
  GtkLayoutManager *layout = gtk_widget_get_layout_manager (box);

  gtk_orientable_set_orientation (GTK_ORIENTABLE (layout), orientation);
  gtk_box_layout_set_spacing (GTK_BOX_LAYOUT (layout), spacing);

  return box;
}

void
sw_box_append (SwBox     *self,
                GtkWidget *child)
{
  g_return_if_fail (SW_IS_BOX (self));
  g_return_if_fail (GTK_IS_WIDGET (child));

  gtk_widget_set_parent (child, GTK_WIDGET (self));
}

void
sw_box_remove (SwBox     *self,
                GtkWidget *child)
{
  g_return_if_fail (SW_IS_BOX (self));
  g_return_if_fail (GTK_IS_WIDGET (child));
  g_return_if_fail (gtk_widget_get_parent (child) == GTK_WIDGET (self));

  gtk_widget_unparent (child);
}
