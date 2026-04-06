#pragma once
#include "../Includes.h"

namespace SDK
{
    struct Screen;

    DEC_MET(Screen_Get_Height, int32_t(*)(MethodInfo* methodInfo), "UnityEngine.CoreModule", "UnityEngine", "Screen", "get_height", 0);
    DEC_MET(Screen_Get_Width, int32_t(*)(MethodInfo* methodInfo), "UnityEngine.CoreModule", "UnityEngine", "Screen", "get_width", 0);
}