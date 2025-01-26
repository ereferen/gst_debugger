#include <cstdio>
#include <gst/gst.h>
#include <memory>

#include "bus_handler/bus_handler.h"
#include "input_controller/input_controller.h"

int main(int argc, char *argv[]) {
  // GStreamerの初期化
  gst_init(&argc, &argv);

  // パイプラインの作成
  GstElement *pipeline = gst_pipeline_new("test-pipeline");
  if (!pipeline) {
    g_printerr("Failed to create pipeline.\n");
    return -1;
  }

  // 各エレメントの作成
  GstElement *source = gst_element_factory_make("videotestsrc", "videotestsrc");
  GstElement *convert =
      gst_element_factory_make("videoconvert", "videoconvert");
  GstElement *sink = gst_element_factory_make("autovideosink", "autovideosink");

  if (!source || !convert || !sink) {
    // std::cerr << "Failed to create one or more elements." << std::endl;
    g_printerr("Failed to create one or more elements.\n");
    if (pipeline)
      gst_object_unref(pipeline);
    return -1;
  }

  /**
   * BUSのメッセージ出力設定
   * - busの取得
   * - busハンドラの作成
   * - busハンドラをbusに登録
   */
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
  auto busHandler = std::make_shared<BusHandler>();
  gst_bus_add_watch(bus, (GstBusFunc)bus_handler::bus_message_callback,
                    busHandler.get());

  // プロパティ設定
  g_object_set(source, "pattern", 18, NULL);

  // エレメントをパイプラインに追加
  gst_bin_add_many(GST_BIN(pipeline), source, convert, sink, nullptr);

  // エレメントをリンク
  if (!gst_element_link(source, convert)) {
    // std::cerr << "Failed to link source and convert." << std::endl;
    g_printerr("Failed to link source and convert.\n");
    gst_object_unref(pipeline);
    return -1;
  }

  if (!gst_element_link(convert, sink)) {
    // std::cerr << "Failed to link convert and sink." << std::endl;
    g_printerr("Failed to link convert and sink.\n");
    gst_object_unref(pipeline);
    return -1;
  }

  // パイプラインの状態を再生に設定
  gst_element_set_state(pipeline, GST_STATE_PLAYING);

  // メインループを作成
  GMainLoop *loop = g_main_loop_new(nullptr, FALSE);
  // std::cout << "Playing video... Press Ctrl+C to stop." << std::endl;
  g_print("Playing video... Press Ctrl+C to stop.\n");

  /**
   * 実行中にキー入力を受け付ける設定
   * - GIOChannelの作成
   * - GIOChannelに対してキー入力を監視する関数を登録
   *   - handle_key_input_for_bus_output : busHandlerに対して入力を処理する関数
   */
  input_controller::show_input_usage();
  GIOChannel *io_channel = g_io_channel_unix_new(fileno(stdin));
  g_io_add_watch(io_channel, G_IO_IN,
                 input_controller::handle_key_input_for_bus_output,
                 busHandler.get());

  // メインループを開始
  g_main_loop_run(loop);

  // 終了処理
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
  g_main_loop_unref(loop);
  g_io_channel_unref(io_channel); // GIOChannelの解放

  return 0;
}
