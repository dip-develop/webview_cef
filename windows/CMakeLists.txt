# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "flutter_chromium")
project(${PROJECT_NAME} LANGUAGES CXX)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
message(STATUS "${CMAKE_BUILD_TYPE}")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/download_cef.cmake)
set(_CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third/cef")
prepare_prebuilt_files(${_CEF_ROOT})

# This value is used when generating builds using this plugin, so it must
# not be changed
set(PLUGIN_NAME "flutter_chromium_plugin")

# Any new source files that you add to the plugin should be added here.
list(APPEND PLUGIN_SOURCES
  "flutter_chromium_plugin.cpp"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_app.cc"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_app.h"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_handler.cc"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_handler.h"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_plugin.cc"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_plugin.h"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_value.cc"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_value.h"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_js_handler.cc"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_js_handler.h"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_cookieVisitor.cc"
  "${CMAKE_CURRENT_LIST_DIR}/../common/webview_cookieVisitor.h"
)

# Define the plugin library target. Its name must not be changed (see comment
# on PLUGIN_NAME above).
add_library(${PLUGIN_NAME} SHARED
  "include/flutter_chromium/flutter_chromium_plugin_c_api.h"
  "flutter_chromium_plugin_c_api.cpp"
  ${PLUGIN_SOURCES}
)

# Apply a standard set of build settings that are configured in the
# application-level CMakeLists.txt. This can be removed for plugins that want
# full control over build settings.
apply_standard_settings(${PLUGIN_NAME})

# Symbols are hidden by default to reduce the chance of accidental conflicts
# between plugins. This should not be removed; any symbols that should be
# exported should be explicitly exported with the FLUTTER_PLUGIN_EXPORT macro.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)

# Source include directories and library dependencies. Add any plugin-specific
# dependencies here.
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${_CEF_ROOT})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)

target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin
debug ${_CEF_ROOT}/debug/libcef.lib
debug ${_CEF_ROOT}/debug/libcef_dll_wrapper.lib
optimized ${_CEF_ROOT}/release/libcef.lib
optimized ${_CEF_ROOT}/release/libcef_dll_wrapper.lib)

# List of absolute paths to libraries that should be bundled with the plugin.
# This list could contain prebuilt libraries, or libraries created by an
# external build triggered from this build file.
set(flutter_chromium_bundled_libraries
  "${_CEF_ROOT}/resources/locales"
  "${_CEF_ROOT}/resources/icudtl.dat"
  "${_CEF_ROOT}/resources/resources.pak"
  "${_CEF_ROOT}/resources/chrome_100_percent.pak"
  "${_CEF_ROOT}/resources/chrome_200_percent.pak"
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/chrome_elf.dll>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/libcef.dll>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/v8_context_snapshot.bin>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/swiftshader>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/d3dcompiler_47.dll>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/libEGL.dll>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/libGLESv2.dll>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/vulkan-1.dll>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/vk_swiftshader.dll>
  $<$<CONFIG:Debug>:${_CEF_ROOT}/debug/bin/vk_swiftshader_icd.json>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/chrome_elf.dll>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/libcef.dll>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/v8_context_snapshot.bin>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/swiftshader>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/d3dcompiler_47.dll>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/libEGL.dll>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/libGLESv2.dll>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/vulkan-1.dll>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/vk_swiftshader.dll>
  $<$<CONFIG:Release>:${_CEF_ROOT}/release/bin/vk_swiftshader_icd.json>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/chrome_elf.dll>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/libcef.dll>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/v8_context_snapshot.bin>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/swiftshader>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/d3dcompiler_47.dll>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/libEGL.dll>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/libGLESv2.dll>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/vulkan-1.dll>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/vk_swiftshader.dll>
  $<$<CONFIG:Profile>:${_CEF_ROOT}/release/bin/vk_swiftshader_icd.json>
    PARENT_SCOPE)

