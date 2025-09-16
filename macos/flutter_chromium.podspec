Pod::Spec.new do |s|
  s.name             = 'flutter_chromium'
  s.version          = '0.0.1'
  s.summary          = 'Flutter webview backed by CEF (Chromium Embedded Framework)'
  s.description      = <<-DESC
Flutter webview backed by CEF (Chromium Embedded Framework)
  DESC
  s.homepage         = 'https://github.com/dip-develop/flutter-chromium'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'DIP Dev' => 'info@dip.dev' }

  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency 'FlutterMacOS'
  s.vendored_frameworks = 'third/cef/Chromium Embedded Framework.framework'
  s.vendored_libraries = 'third/cef/libcef_dll_wrapper.a'

  dir = File.dirname(__FILE__) + "/third/cef/**"
  s.xcconfig = { 'HEADER_SEARCH_PATHS' => dir }
  s.platform = :osx, '10.11'
  s.pod_target_xcconfig = { 'DEFINES_MODULE' => 'YES' }
  s.swift_version = '5.0'
end
