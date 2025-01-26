#include "input_controller.h"
#include "../bus_handler/bus_handler.h"
#include "glib.h"

gboolean input_controller::show_input_usage() {
  g_print("-----BUS OUTPUT CONTROL-----\n");
  g_print("  Input format: set env <type> <true|false>\n");
  g_print("  Input format: get env <type> \n");
  g_print("    <type>: error(0), warning(1), eos(2), state_changed(3), "
          "buffering(4), tag(5), qos(6), "
          "new_clock(7)\n");
  return TRUE;
}

gboolean input_controller::handle_key_input_for_bus_output(
    GIOChannel *source, GIOCondition condition, gpointer data) {
  gchar *str = nullptr;
  if (g_io_channel_read_line(source, &str, nullptr, nullptr, nullptr) ==
      G_IO_STATUS_NORMAL) {
    gchar **tokens = g_strsplit(str, " ", -1);
    guint input_length = g_strv_length(tokens);
    gboolean is_get = g_strcmp0(tokens[0], "get") == 0;
    gboolean is_set = g_strcmp0(tokens[0], "set") == 0;
    gboolean is_env = g_strcmp0(tokens[1], "env") == 0;
    const gchar *type = tokens[2]; // TODO: type validation

    if ((is_get && input_length != 3) || (is_set && input_length != 4) ||
        !is_env) {
      g_strfreev(tokens);
      g_free(str);
      input_controller::show_input_usage();
      return TRUE;
    }

    BusHandler *busHandler = (BusHandler *)data;
    if (is_set) {
      gboolean value = g_strcmp0(tokens[3], "true") == 0 ? TRUE : FALSE;
      busHandler->control_bus_message_env(type, value);
    }

    if (is_get) {
      const gchar *env =
          busHandler->get_bus_message_env(type) ? "true" : "false";
      g_print("%s(type enum): %s\n", type, env);
    }

    g_strfreev(tokens);
  }
  g_free(str);
  return TRUE;
}
