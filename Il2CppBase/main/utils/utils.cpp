#include "utils.h"

namespace fs = std::filesystem;

std::string Utils::GetCheatDirectory()
{
	return "C:\\Il2CppBase";
}

void Utils::CreateCheatDirectory()
{
	const std::string cheatDir = GetCheatDirectory();
	const std::string configDir = cheatDir + "\\configs";

	if (!fs::exists(cheatDir))
	{
		LOG_INFO("Cheat directory not found, creating one.");
		fs::create_directory(cheatDir);
	}
	if (!fs::exists(configDir))
	{
		LOG_INFO("Config directory not found, creating one.");
		fs::create_directory(configDir);
	}
}

std::string Utils::getKeyName(int keyCode)
{
	switch (keyCode)
	{
	case VK_INSERT: return "INSERT";
	case VK_DELETE: return "DELETE";
	case VK_HOME: return "HOME";
	case VK_END: return "END";
	case VK_PRIOR: return "PAGE UP";
	case VK_NEXT: return "PAGE DOWN";
	case VK_F1: return "F1";
	case VK_F2: return "F2";
	case VK_F3: return "F3";
	case VK_F4: return "F4";
	case VK_F5: return "F5";
	case VK_F6: return "F6";
	case VK_F7: return "F7";
	case VK_F8: return "F8";
	case VK_F9: return "F9";
	case VK_F10: return "F10";
	case VK_F11: return "F11";
	case VK_F12: return "F12";
	case VK_RETURN: return "Enter";
	default:
		if (keyCode >= 'A' && keyCode <= 'Z')
			return std::string(1, (char)keyCode);
		else if (keyCode >= '0' && keyCode <= '9')
			return std::string(1, (char)keyCode);
		else
			return "Key " + std::to_string(keyCode);
	}
}

bool Utils::W2S(const SDK::Vector3& worldPos, SDK::Vector3& displayPos)
{
	SDK::Camera* playerCamera = SDK::Camera_get_main(0); if (!playerCamera) return false;

	SDK::Vector3 projected = SDK::Camera_WorldToScreenPoint(playerCamera, worldPos, nullptr); if (projected.Z <= 0.0f) return false;

	float screenHeight = SDK::Screen_Get_Height(nullptr);

	displayPos.X = projected.X;
	displayPos.Y = screenHeight - projected.Y;
	displayPos.Z = projected.Z;

	return true;
}

std::string Utils::UnityStr2SysStr(const SDK::String& string)
{
	int length = string.Fields.StringLength;

	if (length <= 0)
	{
		return "UNKNOWN";
	}

	const uint16_t* chars = &string.Fields.FirstChar;

	std::wstring wideStr(chars, chars + length);
	std::string result;
	result.reserve(length);

	for (wchar_t wc : wideStr)
	{
		if (wc < 0x80)
			result.push_back(static_cast<char>(wc));
		else
		{
			if (wc < 0x800)
			{
				result.push_back(static_cast<char>(0xC0 | (wc >> 6)));
				result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
			}
			else
			{
				result.push_back(static_cast<char>(0xE0 | (wc >> 12)));
				result.push_back(static_cast<char>(0x80 | ((wc >> 6) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | (wc & 0x3F)));
			}
		}
	}

	return result;
}

SDK::String* Utils::SysStr2UnityStr(const std::string& string)
{
	if (!il2cpp_initialize()) return nullptr;

	Il2CppDomain* domain = il2cpp_domain_get();
	if (!domain) return nullptr;

	il2cpp_thread_attach(domain);

	int len = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), nullptr, 0);
	if (len == 0) return nullptr;

	std::wstring wstr(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, string.c_str(), (int)string.size(), &wstr[0], len);

	Il2CppString* il2cppStr = il2cpp_string_new_utf16_wrap((const wchar_t*)wstr.c_str(), len);
	return reinterpret_cast<SDK::String*>(il2cppStr);
}

std::string Utils::GetGameVersion()
{
	auto* u_string = SDK::Application_get_version(0);
	auto version = UnityStr2SysStr(*u_string);
	return version;
}

std::string Utils::GetUnityVersion()
{
	auto* u_string = SDK::Application_get_unityVersion(0);
	auto version = UnityStr2SysStr(*u_string);
	return version;
}