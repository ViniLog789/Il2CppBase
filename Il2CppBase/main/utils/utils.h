#pragma once
#include "Includes.h"

using namespace Il2CppBase; // Your cheat name

namespace Utils
{
	std::string GetCheatDirectory();
	void CreateCheatDirectory();
	std::string getKeyName(int keyCode);
	bool W2S(const SDK::Vector3& worldPos, SDK::Vector3& displayPos);
	std::string UnityStr2SysStr(const SDK::String& string);
	SDK::String* SysStr2UnityStr(const std::string& string);
	std::string GetGameVersion();
	std::string GetUnityVersion();
}