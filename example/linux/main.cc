#include <flutter_chromium/flutter_chromium_plugin.h>
#include "my_application.h"
// Prefer new header; compat include remains available via include/webview_cef/
#include <flutter_chromium/flutter_chromium_plugin.h>

int main(int argc, char** argv) {
  int exit_code = initCEFProcesses(argc, argv);
  if (exit_code >= 0) {
    return exit_code;
  }
  g_autoptr(MyApplication) app = my_application_new();
  return g_application_run(G_APPLICATION(app), argc, argv);
}
