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
namespace bus_handler {
gboolean bus_message_callback(GstBus *bus, GstMessage *message, gpointer data);
}

enum ShowMessageType {
  ERROR,
  WARNING,
  EOS,
  STATE_CHANGED,
  BUFFERING,
  TAG,
  QOS,
  NEW_CLOCK
};

class BusHandler {
public:
  BusHandler(){};
  ~BusHandler(){};

public:
  // バスメッセージハンドラ
  gboolean bus_message_handler(GstMessage *message);
  // 出力制御用変数設定ハンドラ
  gboolean control_bus_message_env(const gchar *str, gboolean value);
  // 出力制御用変数取得ハンドラ
  gboolean get_bus_message_env(const gchar *type);

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
  void set_show_message_error(const gboolean show_message_error);
  gboolean get_show_message_error() const;

  void set_show_message_warning(const gboolean show_message_warning);
  gboolean get_show_message_warning() const;

  void set_show_message_eos(const gboolean show_message_eos);
  gboolean get_show_message_eos() const;

  void
  set_show_message_state_changed(const gboolean show_message_state_changed);
  gboolean get_show_message_state_changed() const;

  void set_show_message_buffering(const gboolean show_message_buffering);
  gboolean get_show_message_buffering() const;

  void set_show_message_tag(const gboolean show_message_tag);
  gboolean get_show_message_tag() const;

  void set_show_message_qos(const gboolean show_message_qos);
  gboolean get_show_message_qos() const;

  void set_show_message_new_clock(const gboolean show_message_new_clock);
  gboolean get_show_message_new_clock() const;
};
#endif // BUS_HANDLER_H