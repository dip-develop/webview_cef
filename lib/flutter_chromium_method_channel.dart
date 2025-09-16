import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'flutter_chromium_platform_interface.dart';

/// An implementation of [WebviewCefPlatform] that uses method channels.
class MethodChannelWebviewCef extends WebviewCefPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('flutter_chromium');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }
}
