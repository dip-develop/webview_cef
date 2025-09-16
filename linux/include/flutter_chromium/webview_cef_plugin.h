#ifndef FLUTTER_PLUGIN_WEBVIEW_CEF_PLUGIN_COMPAT_H_
#define FLUTTER_PLUGIN_WEBVIEW_CEF_PLUGIN_COMPAT_H_

#include "flutter_chromium_plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

// Backward-compat exported C symbols so existing generated registrants still work
FLUTTER_PLUGIN_EXPORT GType webview_cef_plugin_get_type();

FLUTTER_PLUGIN_EXPORT void webview_cef_plugin_register_with_registrar(FlPluginRegistrar* registrar);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // FLUTTER_PLUGIN_WEBVIEW_CEF_PLUGIN_COMPAT_H_
