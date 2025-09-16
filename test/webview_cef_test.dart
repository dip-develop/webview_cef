import 'package:flutter_test/flutter_test.dart';
// import 'package:flutter_chromium/flutter_chromium.dart'; // Not used: wrapper class not present in this package
import 'package:flutter_chromium/flutter_chromium_platform_interface.dart';
import 'package:flutter_chromium/flutter_chromium_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockWebviewCefPlatform
    with MockPlatformInterfaceMixin
    implements WebviewCefPlatform {
  @override
  Future<String?> getPlatformVersion() => Future.value('42');
}

void main() {
  final WebviewCefPlatform initialPlatform = WebviewCefPlatform.instance;

  test('$MethodChannelWebviewCef is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelWebviewCef>());
  });

  test('getPlatformVersion', () async {
    MockWebviewCefPlatform fakePlatform = MockWebviewCefPlatform();
    WebviewCefPlatform.instance = fakePlatform;

    expect(await WebviewCefPlatform.instance.getPlatformVersion(), '42');
  });
}
