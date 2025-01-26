#ifndef BUS_HANDLER_H
#define BUS_HANDLER_H

#include "gst/gstbus.h"
#include <glib-2.0/glib.h>
#include <gst/gst.h>

/**
 * pipelineのバスメッセージを処理する関数群
 * bus_message_handler: バスメッセージを処理する関数
 *   これが呼ばれると、メッセージの種類に応じて適切な関数を呼び出す
 *
 * 追加で新たなメッセージタイプを処理したい場合はここに定義し、cppファイルに実装する
 */

/**
 * MEMO: struct GstMessage from gstmessage.h
 *struct _GstMessage
 * {
 *   GstMiniObject   mini_object;
 *
 *   //< public >
 *   GstMessageType  type;
 *   guint64         timestamp;
 *   GstObject      *src;
 *   guint32         seqnum;
 *
 *   //< private >
 *   GMutex          lock;
 *   GCond           cond;
 * };
 *
 * MEMO: enum GstMessageType from gstreamer documentation
 * https://gstreamer.freedesktop.org/documentation/gstreamer/gstmessage.html#GstMessageType
 */

gboolean bus_message_callback(GstBus *bus, GstMessage *message, gpointer data);

class BusHandler {
public:
  BusHandler(){};
  ~BusHandler(){};

public:
  // バスメッセージハンドラ
  gboolean bus_message_handler(GstMessage *message);

private:
  // メッセージハンドラ
  void bus_message_error(GstMessage *message);
  void bus_message_warning(GstMessage *message);
  void bus_message_eos(GstMessage *message);
  void bus_message_state_changed(GstMessage *message);
  void bus_message_buffering(GstMessage *message);
  void bus_message_tag(GstMessage *message);
  void bus_message_qos(GstMessage *message);
  void bus_message_new_clock(GstMessage *message);

private:
  // 制御用変数
  gboolean show_message_error = TRUE;
  gboolean show_message_warning = TRUE;
  gboolean show_message_eos = TRUE;
  gboolean show_message_state_changed = TRUE;
  gboolean show_message_buffering = TRUE;
  gboolean show_message_tag = TRUE;
  gboolean show_message_qos = TRUE;
  gboolean show_message_new_clock = TRUE;

public:
  // メッセージ表示設定
  void set_show_message_error(gboolean show_message_error);
  gboolean get_show_message_error();

  void set_show_message_warning(gboolean show_message_warning);
  gboolean get_show_message_warning();

  void set_show_message_eos(gboolean show_message_eos);
  gboolean get_show_message_eos();

  void set_show_message_state_changed(gboolean show_message_state_changed);
  gboolean get_show_message_state_changed();

  void set_show_message_buffering(gboolean show_message_buffering);
  gboolean get_show_message_buffering();

  void set_show_message_tag(gboolean show_message_tag);
  gboolean get_show_message_tag();

  void set_show_message_qos(gboolean show_message_qos);
  gboolean get_show_message_qos();

  void set_show_message_new_clock(gboolean show_message_new_clock);
  gboolean get_show_message_new_clock();
};
#endif // BUS_HANDLER_H