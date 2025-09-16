// New-named Windows plugin implementation, migrated from legacy WebviewCefPlugin
// with minimal behavior change.
#include "include/flutter_chromium/flutter_chromium_plugin.h"
#include "flutter_chromium_keyevent.h"
// This must be included before many other Windows headers.
#include <windows.h>

#include <flutter/method_channel.h>
#include <flutter_windows.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <thread>
#include <iostream>
#include <mutex>

#include "../common/webview_plugin.h"
#include "../common/webview_value.h"

namespace flutter_chromium {

class WebviewTextureRenderer : public webview_cef::WebviewTexture{
 public:
	explicit WebviewTextureRenderer(FlutterDesktopTextureRegistrarRef texture_registrar) {
		registrar_ = texture_registrar;
		texture = std::make_unique<flutter::TextureVariant>(
				flutter::PixelBufferTexture([this](size_t width, size_t height) -> const FlutterDesktopPixelBuffer* {
					return this->CopyPixelBuffer(width, height);
				}));
		FlutterDesktopTextureInfo info = {};
		info.type = kFlutterDesktopPixelBufferTexture;
		info.pixel_buffer_config.user_data = std::get_if<flutter::PixelBufferTexture>(texture.get());
		info.pixel_buffer_config.callback = [](size_t width, size_t height, void* user_data) -> const FlutterDesktopPixelBuffer* {
			auto texture = static_cast<flutter::PixelBufferTexture*>(user_data);
			return texture->CopyPixelBuffer(width, height);
		};
		textureId = FlutterDesktopTextureRegistrarRegisterExternalTexture(registrar_, &info);
	}

	~WebviewTextureRenderer() override {
		std::lock_guard<std::mutex> autolock(mutex_);
		if (registrar_) {
			// FlutterDesktopTextureRegistrarUnregisterExternalTexture(registrar_, textureId, nullptr, nullptr);
		}
	}

	const FlutterDesktopPixelBuffer* CopyPixelBuffer(size_t width, size_t height) const {
		std::lock_guard<std::mutex> autolock(mutex_);
		return pixel_buffer.get();
	}

	void onFrame(const void* buffer, int width, int height) override {
		const std::lock_guard<std::mutex> autolock(mutex_);
		if (!pixel_buffer.get() || pixel_buffer.get()->width != width || pixel_buffer.get()->height != height) {
			if (!pixel_buffer.get()) {
				pixel_buffer = std::make_unique<FlutterDesktopPixelBuffer>();
				pixel_buffer->release_context = nullptr;
			}
			pixel_buffer->width = width;
			pixel_buffer->height = height;
			const auto size = width * height * 4;
			backing_pixel_buffer.reset(new uint8_t[size]);
			pixel_buffer->buffer = backing_pixel_buffer.get();
		}

		webview_cef::SwapBufferFromBgraToRgba((void*)pixel_buffer->buffer, buffer, width, height);
		if (registrar_) {
			FlutterDesktopTextureRegistrarMarkExternalTextureFrameAvailable(registrar_, textureId);
		}
	}

	FlutterDesktopTextureRegistrarRef registrar_;
	std::unique_ptr<flutter::TextureVariant> texture;
	mutable std::shared_ptr<FlutterDesktopPixelBuffer> pixel_buffer;
	std::unique_ptr<uint8_t> backing_pixel_buffer;
	mutable std::mutex mutex_;
	int64_t textureId{0};
};

static flutter::EncodableValue encode_wvalue_to_flvalue(webview_cef::WValue* args) {
	using namespace webview_cef;
	WValueType type = webview_value_get_type(args);
	switch (type) {
		case Webview_Value_Type_Bool:
			return flutter::EncodableValue(webview_value_get_bool(args));
		case Webview_Value_Type_Int:
			return flutter::EncodableValue(webview_value_get_int(args));
		case Webview_Value_Type_Float:
			return flutter::EncodableValue(webview_value_get_float(args));
		case Webview_Value_Type_Double:
			return flutter::EncodableValue(webview_value_get_double(args));
		case Webview_Value_Type_String:
			return flutter::EncodableValue(webview_value_get_string(args));
		case Webview_Value_Type_Uint8_List:
			return flutter::EncodableValue(webview_value_get_uint8_list(args));
		case Webview_Value_Type_Int32_List:
			return flutter::EncodableValue(webview_value_get_int32_list(args));
		case Webview_Value_Type_Int64_List:
			return flutter::EncodableValue(webview_value_get_int64_list(args));
		case Webview_Value_Type_Float_List:
			return flutter::EncodableValue(webview_value_get_float_list(args));
		case Webview_Value_Type_Double_List:
			return flutter::EncodableValue(webview_value_get_double_list(args));
		case Webview_Value_Type_List: {
			flutter::EncodableList ret;
			size_t len = webview_value_get_len(args);
			for (size_t i = 0; i < len; i++) {
				ret.push_back(encode_wvalue_to_flvalue(webview_value_get_list_value(args, i)));
			}
			return ret;
		}
		case Webview_Value_Type_Map: {
			flutter::EncodableMap ret;
			size_t len = webview_value_get_len(args);
			for (size_t i = 0; i < len; i++) {
				ret[encode_wvalue_to_flvalue(webview_value_get_key(args, i))] = encode_wvalue_to_flvalue(webview_value_get_value(args, i));
			}
			return ret;
		}
		default:
			return flutter::EncodableValue(nullptr);
	}
}

static webview_cef::WValue* encode_flvalue_to_wvalue(flutter::EncodableValue* args) {
	using namespace webview_cef;
	size_t index = args->index();
	if (index == 1) {
		return webview_value_new_bool(*std::get_if<bool>(args));
	} else if (index == 2 || index == 3) {
		return webview_value_new_int(*std::get_if<int32_t>(args));
	} else if (index == 4) {
		return webview_value_new_double(*std::get_if<double>(args));
	} else if (index == 5) {
		return webview_value_new_string((*std::get_if<std::string>(args)).c_str());
	} else if (index == 6) {
		auto list = *std::get_if<std::vector<uint8_t>>(args);
		return webview_value_new_uint8_list(list.data(), list.size());
	} else if (index == 7) {
		auto list = *std::get_if<std::vector<int32_t>>(args);
		return webview_value_new_int32_list(list.data(), list.size());
	} else if (index == 8) {
		auto list = *std::get_if<std::vector<int64_t>>(args);
		return webview_value_new_int64_list(list.data(), list.size());
	} else if (index == 9) {
		auto list = *std::get_if<std::vector<double>>(args);
		return webview_value_new_double_list(list.data(), list.size());
	} else if (index == 10) {
		WValue* ret = webview_value_new_list();
		flutter::EncodableList list = *std::get_if<flutter::EncodableList>(args);
		for (size_t i = 0; i < list.size(); i++) {
			WValue* value = encode_flvalue_to_wvalue(&list[i]);
			webview_value_append(ret, value);
			webview_value_unref(value);
		}
		return ret;
	} else if (index == 11) {
		WValue* ret = webview_value_new_map();
		flutter::EncodableMap map = *std::get_if<flutter::EncodableMap>(args);
		for (flutter::EncodableMap::iterator it = map.begin(); it != map.end(); it++) {
			WValue* key = encode_flvalue_to_wvalue(const_cast<flutter::EncodableValue*>(&it->first));
			WValue* value = encode_flvalue_to_wvalue(const_cast<flutter::EncodableValue*>(&it->second));
			webview_value_set(ret, key, value);
			webview_value_unref(key);
			webview_value_unref(value);
		}
		return ret;
	} else if (index == 12) {
		return nullptr;
	} else if (index == 13) {
		auto list = *std::get_if<std::vector<float>>(args);
		return webview_value_new_float_list(list.data(), list.size());
	}
	return nullptr;
}

static std::unordered_map<HWND, std::shared_ptr<webview_cef::WebviewPlugin>> webviewPlugins;
static std::unordered_map<HWND, std::function<void(std::string method, flutter::EncodableValue* arguments)>> webviewChannels;

// FlutterChromiumPlugin
class FlutterChromiumPlugin::Impl {
 public:
	Impl() = default;
	~Impl() = default;

	void HandleMethodCall(const flutter::MethodCall<flutter::EncodableValue>& method_call,
												std::shared_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
		webview_cef::WValue* encodeArgs = encode_flvalue_to_wvalue(const_cast<flutter::EncodableValue*>(method_call.arguments()));
		plugin_->HandleMethodCall(method_call.method_name(), encodeArgs, [result](int ret, webview_cef::WValue* args) {
			if (ret > 0) {
				result->Success(encode_wvalue_to_flvalue(args));
			} else if (ret < 0) {
				result->Error("error", "error", encode_wvalue_to_flvalue(args));
			} else {
				result->NotImplemented();
			}
		});
		webview_cef::webview_value_unref(encodeArgs);
	}

	std::shared_ptr<webview_cef::WebviewPlugin> plugin_;
	FlutterDesktopTextureRegistrarRef texture_registrar_{nullptr};
	std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> channel_;
	HWND hwnd_{nullptr};
};

void FlutterChromiumPlugin::RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar) {
	auto plugin = std::make_unique<FlutterChromiumPlugin>();
	plugin->impl_->texture_registrar_ = registrar->texture_registrar();
	plugin->impl_->channel_ = std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
			registrar->messenger(), "flutter_chromium", &flutter::StandardMethodCodec::GetInstance());

	plugin->impl_->channel_->SetMethodCallHandler(
			[plugin_pointer = plugin.get()](const auto& call, auto result) { plugin_pointer->impl_->HandleMethodCall(call, std::move(result)); });

	plugin->impl_->hwnd_ = registrar->GetView()->GetNativeWindow();
	webviewPlugins.emplace(plugin->impl_->hwnd_, plugin->impl_->plugin_);
	webviewChannels.emplace(plugin->impl_->hwnd_, [plugin_pointer = plugin.get()](std::string method, flutter::EncodableValue* arguments) {
		plugin_pointer->impl_->channel_->InvokeMethod(method, std::make_unique<flutter::EncodableValue>(*arguments));
	});
	plugin->impl_->plugin_->setInvokeMethodFunc([plugin_pointer = plugin.get()](std::string method, webview_cef::WValue* arguments) {
		auto* methodValue = new flutter::EncodableValue(method);
		auto* args = new flutter::EncodableValue(encode_wvalue_to_flvalue(arguments));
		PostMessage(plugin_pointer->impl_->hwnd_, WM_USER + 1, WPARAM(methodValue), LPARAM(args));
	});

	plugin->impl_->plugin_->setCreateTextureFunc([plugin_pointer = plugin.get()]() {
		std::shared_ptr<WebviewTextureRenderer> renderer = std::make_shared<WebviewTextureRenderer>(plugin_pointer->impl_->texture_registrar_);
		return std::dynamic_pointer_cast<webview_cef::WebviewTexture>(renderer);
	});

	registrar->AddPlugin(std::move(plugin));
}

FlutterChromiumPlugin::FlutterChromiumPlugin() : impl_(std::make_unique<Impl>()) {
	impl_->plugin_ = std::make_shared<webview_cef::WebviewPlugin>();
}

FlutterChromiumPlugin::~FlutterChromiumPlugin() {
	impl_->plugin_ = nullptr;
	webviewPlugins.erase(impl_->hwnd_);
	webviewChannels.erase(impl_->hwnd_);
	if (webviewPlugins.empty()) {
		webview_cef::stopCEF();
	}
}

// Back-compat wndproc handler used by example host
extern "C" __declspec(dllexport) void handleWndProcForCEF(HWND hwnd, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
	switch (message) {
		case WM_USER + 1: {
			if (webviewPlugins.find(hwnd) != webviewPlugins.end()) {
				auto* method = (flutter::EncodableValue*)wParam;
				auto* args = (flutter::EncodableValue*)lParam;
				webviewChannels[hwnd]((*std::get_if<std::string>(method)), args);
			}
			break;
		}
		case WM_SYSCHAR:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR: {
			if (webviewPlugins.find(hwnd) != webviewPlugins.end()) {
				CefKeyEvent keyEvent = getCefKeyEvent(message, wParam, lParam);
				webviewPlugins[hwnd]->sendKeyEvent(keyEvent);
			}
		}
	}
}

}  // namespace flutter_chromium
