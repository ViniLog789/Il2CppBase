#pragma once
#include "Includes.h"

namespace Il2CppBase // Your cheat name
{
	struct Hooks
	{
		// Hook menu
		static HRESULT HkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		static LRESULT HkWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Other hooks

	};
}
