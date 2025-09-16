#ifndef FLUTTER_CHROMIUM_PLUGIN_H_
#define FLUTTER_CHROMIUM_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FLUTTER_PLUGIN_EXPORT __declspec(dllimport)
#endif

namespace flutter_chromium {

class FlutterChromiumPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  FlutterChromiumPlugin();
  virtual ~FlutterChromiumPlugin();

  // Disallow copy and assign.
  FlutterChromiumPlugin(const FlutterChromiumPlugin&) = delete;
  FlutterChromiumPlugin& operator=(const FlutterChromiumPlugin&) = delete;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};

}  // namespace flutter_chromium

#endif  // FLUTTER_CHROMIUM_PLUGIN_H_
