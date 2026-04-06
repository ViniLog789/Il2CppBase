#pragma once
#include "il2cpp/il2cpp.h"
#include "main/logger/logging.h" // For LOGS

using namespace Il2CppBase; // your cheat name

// EXPAMPLE USE: DEC_MET(Behaviour_Set_Enabled, void(*)(Behaviour* behaviour, bool enabled, MethodInfo* methodInfo), "UnityEngine.CoreModule", "UnityEngine", "Behaviour", "set_enabled", 1);
#define DEC_MET(NAME, TYPE, ASSEMBLY, NAMESPACE, CLASS, METHOD, ARGCOUNT) \
using NAME##_ptr = TYPE; \
inline NAME##_ptr NAME = reinterpret_cast<NAME##_ptr>(il2cpp_get_method_pointer(ASSEMBLY, NAMESPACE, CLASS, METHOD, ARGCOUNT));

namespace SDK
{
    inline bool Initialize()
    {
        LOG_INFO("Initializing IL2CPP SDK...");
        LOG_INFO("GameAssembly.dll base address: ", (GetModuleHandleA("GameAssembly.dll")));
        return il2cpp_initialize();
    }
}

#include "MethodInfo.h"
#include "MonoBehaviour.h"
#include "Cursor.h"
#include "String.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "Camera.h"
#include "Screen.h"
#include "Application.h"
#include "System.h"
#include "Transform.h"
#include "Object.h"
#include "GameObject.h"
#include "Component.h"