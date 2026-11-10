/* sw-label.c */

#define SW_COMPILATION
#include "sw-label.h"

struct _SwLabel
{
  SwWidget   parent_instance;

  GtkWidget *inner_label; /* owned via gtk_widget_set_parent() */
};

G_DEFINE_TYPE (SwLabel, sw_label, SW_TYPE_WIDGET)

static void
sw_label_dispose (GObject *object)
{
  SwLabel *self = SW_LABEL (object);

  g_clear_pointer (&self->inner_label, gtk_widget_unparent);

  G_OBJECT_CLASS (sw_label_parent_class)->dispose (object);
}

static void
sw_label_class_init (SwLabelClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->dispose = sw_label_dispose;

  /* A single-child bin: SwLabel composes an internal GtkLabel for text
   * shaping/layout, which is outsourced to Pango via GTK per
   * docs/proposal/0001-dependency-ownership-policy.md section 3 rather
   * than reimplemented here. */
  gtk_widget_class_set_layout_manager_type (widget_class, GTK_TYPE_BIN_LAYOUT);
  gtk_widget_class_set_accessible_role (widget_class, GTK_ACCESSIBLE_ROLE_LABEL);
  gtk_widget_class_set_css_name (widget_class, "sw-label");
}

static void
sw_label_init (SwLabel *self)
{
  self->inner_label = gtk_label_new (NULL);
  gtk_widget_set_parent (self->inner_label, GTK_WIDGET (self));
}

GtkWidget *
sw_label_new (const char *text)
{
  GtkWidget *label = g_object_new (SW_TYPE_LABEL, NULL);

  sw_label_set_text (SW_LABEL (label), text);

  return label;
}

void
sw_label_set_text (SwLabel    *self,
                    const char *text)
{
  g_return_if_fail (SW_IS_LABEL (self));

  gtk_label_set_text (GTK_LABEL (self->inner_label), text != NULL ? text : "");
}

const char *
sw_label_get_text (SwLabel *self)
{
  g_return_val_if_fail (SW_IS_LABEL (self), NULL);

  return gtk_label_get_text (GTK_LABEL (self->inner_label));
}
