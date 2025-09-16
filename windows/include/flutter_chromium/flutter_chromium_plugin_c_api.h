#ifndef FLUTTER_CHROMIUM_PLUGIN_C_API_H_
#define FLUTTER_CHROMIUM_PLUGIN_C_API_H_

#include <flutter/flutter_plugin_registrar.h>
#include <windows.h>

#ifdef FLUTTER_PLUGIN_IMPL
#define FLUTTER_PLUGIN_EXPORT __declspec(dllexport)
#else
#define FLUTTER_PLUGIN_EXPORT __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

FLUTTER_PLUGIN_EXPORT void FlutterChromiumPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar);

// Preferred C API for initializing CEF processes and forwarding Win32 messages.
// These mirror the legacy exports to ease migration for host apps.
FLUTTER_PLUGIN_EXPORT int initCEFProcesses(HINSTANCE hInstance);
FLUTTER_PLUGIN_EXPORT void handleWndProcForCEF(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // FLUTTER_CHROMIUM_PLUGIN_C_API_H_
