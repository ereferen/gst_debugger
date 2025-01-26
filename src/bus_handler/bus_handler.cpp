#include "bus_handler.h"
#include "glib.h"
#include <cstdlib>

// バスメッセージコールバック
gboolean bus_handler::bus_message_callback(GstBus *bus, GstMessage *message,
                                           gpointer data) {
  BusHandler *busHandler = (BusHandler *)data;
  return busHandler->bus_message_handler(message);
}

// バスメッセージハンドラ
gboolean BusHandler::bus_message_handler(GstMessage *message) {
  switch (GST_MESSAGE_TYPE(message)) {
  case GST_MESSAGE_ERROR:
    bus_message_error(message);
    break;
  case GST_MESSAGE_WARNING:
    bus_message_warning(message);
    break;
  case GST_MESSAGE_EOS:
    bus_message_eos(message);
    break;
  case GST_MESSAGE_STATE_CHANGED:
    bus_message_state_changed(message);
    break;
  case GST_MESSAGE_BUFFERING:
    bus_message_buffering(message);
    break;
  case GST_MESSAGE_TAG:
    bus_message_tag(message);
    break;
  case GST_MESSAGE_QOS:
    bus_message_qos(message);
    break;
  case GST_MESSAGE_NEW_CLOCK:
    bus_message_new_clock(message);
    break;
  default:
    g_print("Undefined message type. GST_MESSAGE_TYPE: %d\n",
            GST_MESSAGE_TYPE(message));
    break;
  }

  return TRUE;
}

// エラーメッセージハンドラ
void BusHandler::bus_message_error(GstMessage *message) {
  if (!this->show_message_error) {
    return;
  }
  GError *err;
  gchar *debug_info;

  gst_message_parse_error(message, &err, &debug_info);
  g_printerr("Error: %s\n", err->message);
  g_error_free(err);
  g_free(debug_info);
}

// 警告メッセージハンドラ
void BusHandler::bus_message_warning(GstMessage *message) {
  if (!this->show_message_warning) {
    return;
  }
  GError *err;
  gchar *debug_info;

  gst_message_parse_warning(message, &err, &debug_info);
  g_printerr("Warning: %s\n", err->message);
  g_error_free(err);
  g_free(debug_info);
}

// EOSメッセージハンドラ
void BusHandler::bus_message_eos(GstMessage *message) {
  if (!this->show_message_eos) {
    return;
  }
  g_print("End of stream.\n");
}

// ステータス変更メッセージハンドラ
void BusHandler::bus_message_state_changed(GstMessage *message) {
  if (!this->show_message_state_changed) {
    return;
  }
  GstState old_state, new_state, pending_state;
  gst_message_parse_state_changed(message, &old_state, &new_state,
                                  &pending_state);

  // 変更された要素の名前を取得
  const gchar *element_name = GST_OBJECT_NAME(message->src);

  g_print("[%s]State changed from %s to %s\n", element_name,
          gst_element_state_get_name(old_state),
          gst_element_state_get_name(new_state));
}

// バッファリングメッセージハンドラ
void BusHandler::bus_message_buffering(GstMessage *message) {
  if (!this->show_message_buffering) {
    return;
  }
  gint percent;
  gst_message_parse_buffering(message, &percent);
  g_print("Buffering (%d%%)\n", percent);
}

// タグメッセージハンドラ
void BusHandler::bus_message_tag(GstMessage *message) {
  if (!this->show_message_tag) {
    return;
  }
  GstTagList *tags;
  gst_message_parse_tag(message, &tags);
  g_print("Tag message received.\n");
}

// QoSメッセージハンドラ
void BusHandler::bus_message_qos(GstMessage *message) {
  if (!this->show_message_qos) {
    return;
  }
  g_print("QoS message received.\n");
}

// 新しいクロックメッセージハンドラ
void BusHandler::bus_message_new_clock(GstMessage *message) {
  if (!this->show_message_new_clock) {
    return;
  }
  g_print("New clock message received.\n");
}

gboolean BusHandler::control_bus_message_env(const gchar *str, gboolean value) {
  g_print("Received input: %s\n", str);
  switch (atoi(str)) {
  case ShowMessageType::ERROR:
    set_show_message_error(value);
    break;
  case ShowMessageType::WARNING:
    set_show_message_warning(value);
    break;
  case ShowMessageType::EOS:
    set_show_message_eos(value);
    break;
  case ShowMessageType::STATE_CHANGED:
    set_show_message_state_changed(value);
    break;
  case ShowMessageType::BUFFERING:
    set_show_message_buffering(value);
    break;
  case ShowMessageType::TAG:
    set_show_message_tag(value);
    break;
  case ShowMessageType::QOS:
    set_show_message_qos(value);
    break;
  case ShowMessageType::NEW_CLOCK:
    set_show_message_new_clock(value);
    break;
  default:
    g_printerr("Undefined message type. ShowMessageType: %d\n", atoi(str));
    break;
  }
  return TRUE;
}

gboolean BusHandler::get_bus_message_env(const gchar *type) {
  switch (atoi(type)) {
  case ShowMessageType::ERROR:
    return get_show_message_error();
  case ShowMessageType::WARNING:
    return get_show_message_warning();
  case ShowMessageType::EOS:
    return get_show_message_eos();
  case ShowMessageType::STATE_CHANGED:
    return get_show_message_state_changed();
  case ShowMessageType::BUFFERING:
    return get_show_message_buffering();
  case ShowMessageType::TAG:
    return get_show_message_tag();
  case ShowMessageType::QOS:
    return get_show_message_qos();
  case ShowMessageType::NEW_CLOCK:
    return get_show_message_new_clock();
  default:
    g_printerr("Undefined message type. ShowMessageType: %d\n", atoi(type));
    return FALSE;
  }
}

// メッセージ表示設定
void BusHandler::set_show_message_error(const gboolean show_message_error) {
  this->show_message_error = show_message_error;
}

gboolean BusHandler::get_show_message_error() const {
  return this->show_message_error;
}

void BusHandler::set_show_message_warning(const gboolean show_message_warning) {
  this->show_message_warning = show_message_warning;
}

gboolean BusHandler::get_show_message_warning() const {
  return this->show_message_warning;
}

void BusHandler::set_show_message_eos(const gboolean show_message_eos) {
  this->show_message_eos = show_message_eos;
}

gboolean BusHandler::get_show_message_eos() const {
  return this->show_message_eos;
}

void BusHandler::set_show_message_state_changed(
    const gboolean show_message_state_changed) {
  this->show_message_state_changed = show_message_state_changed;
}

gboolean BusHandler::get_show_message_state_changed() const {
  return this->show_message_state_changed;
}

void BusHandler::set_show_message_buffering(
    const gboolean show_message_buffering) {
  this->show_message_buffering = show_message_buffering;
}

gboolean BusHandler::get_show_message_buffering() const {
  return this->show_message_buffering;
}

void BusHandler::set_show_message_tag(const gboolean show_message_tag) {
  this->show_message_tag = show_message_tag;
}

gboolean BusHandler::get_show_message_tag() const {
  return this->show_message_tag;
}

void BusHandler::set_show_message_qos(const gboolean show_message_qos) {
  this->show_message_qos = show_message_qos;
}

gboolean BusHandler::get_show_message_qos() const {
  return this->show_message_qos;
}

void BusHandler::set_show_message_new_clock(
    const gboolean show_message_new_clock) {
  this->show_message_new_clock = show_message_new_clock;
}

gboolean BusHandler::get_show_message_new_clock() const {
  return this->show_message_new_clock;
}
