# Migration: webview_cef → flutter_chromium

This guide helps migrate existing apps and forks from `webview_cef` to `flutter_chromium`.

## Package and imports

- Dart package name changed:
  - Before: `package:webview_cef/...`
  - After: `package:flutter_chromium/...`
- Public entry remains similar; recommended import:
  - `import 'package:flutter_chromium/flutter_chromium.dart';`

## Method channel

- Unified method channel name to `flutter_chromium` across all platforms.
  - If you called into platform channels directly, update the channel string.

## Initialization

- Call `WebviewManager().initialize(...)` once before creating any WebViews.
- Initialization is idempotent and guarded with a timeout. Errors surface via `PlatformException`.
- New options:
  - `cachePath`: enables persistence when non-empty.
  - `persistSessionCookies` and `persistUserPreferences`: only effective with a non-empty `cachePath`.
  - `enableGPU`: toggles GPU acceleration.

## Linux headers and symbols

- Linux plugin target is `flutter_chromium_plugin`.
- Backward-compatible headers/symbols are provided so generated registrants that reference `webview_cef` continue to work:
  - `include/webview_cef/webview_cef_plugin.h` → shim to `flutter_chromium/flutter_chromium_plugin.h`.
  - Legacy exported C symbols are aliased to the new names.

## CEF binaries location

- Per-OS storage only (legacy `third/cef` fallback removed):
  - Linux: `linux/third/cef`
  - Windows: `windows/third/cef`
  - macOS: `macos/third/cef` (via CocoaPods/podspec vendored frameworks/libs)
- Linux (only): choose CEF flavor via CMake:
  - `DOWNLOAD_CEF_FLAVOR=standard` (default)
  - `DOWNLOAD_CEF_FLAVOR=minimal`

## Breaking changes to watch for

- Method channel name is now `flutter_chromium`.
- Example code and docs reference the new package name and include path.
- If you patched CMake files previously to hard-code `third/cef`, update to the new per-OS directories. The legacy fallback has been removed.

## Windows/macOS notes

- Channel name updated; APIs and setup otherwise remain compatible with the original plugin patterns.

## Licensing

- Original project remains under Apache-2.0 (LICENSE preserved).
- New/modified work in this fork is offered under MIT. See `NOTICE` and `LICENSE-MIT` for details.
