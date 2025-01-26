#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <glib-2.0/glib.h>
#include <gst/gst.h>

namespace input_controller {
gboolean show_input_usage();
gboolean handle_key_input_for_bus_output(GIOChannel *source,
                                         GIOCondition condition, gpointer data);
} // namespace input_controller

#endif // INPUT_CONTROLLER_H