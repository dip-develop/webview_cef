#
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html.
# Run `pod lib lint webview_cef.podspec` to validate before publishing.
#
Pod::Spec.new do |s|
  s.name             = 'webview_cef' # DEPRECATED: use flutter_chromium.podspec instead
  s.version          = '0.0.1'
  s.summary          = 'Flutter webview backed by CEF (Chromium Embedded Framework)'
  s.description      = <<-DESC
Flutter webview backed by CEF (Chromium Embedded Framework)
                       DESC
  s.homepage         = 'https://github.com/dip-develop/flutter-chromium'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'DIP Dev' => 'info@dip.dev' }

  s.source           = { :path => '.' }
  # Compile only the new FlutterChromium plugin and required wrappers; exclude legacy WebviewCefPlugin.*
  # Intentionally limit sources to prevent duplicate registration.
  s.source_files     = 'Classes/FlutterChromiumPlugin.*', 'Classes/CefWrapper.*', 'Classes/WebviewCefTexture.*', 'Classes/cef_bridge.cc'
  s.dependency 'FlutterMacOS'
  s.vendored_frameworks = 'third/cef/Chromium Embedded Framework.framework'
  s.vendored_libraries = 'third/cef/libcef_dll_wrapper.a'

  $dir = File.dirname(__FILE__)
  $dir = $dir + "/third/cef/**"
  s.xcconfig = { "HEADER_SEARCH_PATHS" => $dir}
  # s.private_header_files = '../common/simple_app.h', '../common/simple_handler.h'

  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
end
