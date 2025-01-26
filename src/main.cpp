#include <gst/gst.h>
#include <iostream>
#include <memory>

#include "bus_handler/bus_handler.h" // Ensure BusHandler is fully declared
#include "glib.h"
#include "gst/gstbus.h"

int main(int argc, char *argv[]) {
  // GStreamerの初期化
  gst_init(&argc, &argv);

  // パイプラインの作成
  GstElement *pipeline = gst_pipeline_new("test-pipeline");
  if (!pipeline) {
    std::cerr << "Failed to create pipeline." << std::endl;
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

  // BUSの取得
  GstBus *bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));

  // バスハンドラの作成
  auto busHandler = std::make_shared<BusHandler>();
  gst_bus_add_watch(bus, (GstBusFunc)bus_message_callback, busHandler.get());

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

  // メインループを開始
  g_main_loop_run(loop);

  // 終了処理
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);
  g_main_loop_unref(loop);

  return 0;
}
