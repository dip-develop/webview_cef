#include "include/flutter_chromium/flutter_chromium_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>
#include "include/flutter_chromium/flutter_chromium_plugin.h"

using flutter_chromium::FlutterChromiumPlugin;

void FlutterChromiumPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  auto* windows_registrar =
      flutter::PluginRegistrarManager::GetInstance()->GetRegistrar<flutter::PluginRegistrarWindows>(registrar);
  FlutterChromiumPlugin::RegisterWithRegistrar(windows_registrar);
}

// Provide exports that forward to internal implementations to avoid relying on legacy DLL symbols.
extern "C" {
FLUTTER_PLUGIN_EXPORT int initCEFProcesses(HINSTANCE hInstance) {
  // Invoke the CEF init from common code.
  return webview_cef::initCEFProcesses(hInstance);
}
}
