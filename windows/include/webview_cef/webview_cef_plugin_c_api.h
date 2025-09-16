// Back-compat header for legacy C API. Prefer including
// `include/flutter_chromium/flutter_chromium_plugin_c_api.h` in new code.
#ifndef FLUTTER_PLUGIN_WEBVIEW_CEF_PLUGIN_C_API_H_
#define FLUTTER_PLUGIN_WEBVIEW_CEF_PLUGIN_C_API_H_

#include <flutter/plugin_registrar_windows.h>
#include <windows.h>

#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FLUTTER_PLUGIN_EXPORT __declspec(dllimport)
#endif

#if defined(__cplusplus)
extern "C" {
#endif

// Legacy registration entrypoint preserved for binary compatibility.
FLUTTER_PLUGIN_EXPORT void WebviewCefPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar);

// Legacy helper exports used by host apps that integrate CEF process/wndproc.
FLUTTER_PLUGIN_EXPORT int initCEFProcesses(HINSTANCE hInstance);
FLUTTER_PLUGIN_EXPORT void handleWndProcForCEF(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);

#if defined(__cplusplus)
}  // extern "C"
#endif

#endif  // FLUTTER_PLUGIN_WEBVIEW_CEF_PLUGIN_C_API_H_
