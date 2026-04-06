#include "Includes.h"
#include "memory.h"

using namespace Il2CppBase;

static std::unique_ptr<Logger> loggerInstance;
static std::unique_ptr<Renderer> rendererInstance;
static std::unique_ptr<Hooking> hookingInstance;
static std::unique_ptr<FeatureHandler> featureInstance;

// Main cheat thread
extern "C" __declspec(dllexport) DWORD WINAPI Il2CppBaseThread() // your cheat name
{
    Utils::CreateCheatDirectory();

    bool hooksApplied = false;

    try {
        loggerInstance = std::make_unique<Logger>(Logger::Level::Call);
    }
    catch (...) {
        return 0;
    }

    // Initialize SDK
    if (!SDK::Initialize()) {
        LOG_ERROR("Failed to initialize SDK");
        goto finalize;
    }

    try {
        rendererInstance = std::make_unique<Renderer>();
        hookingInstance = std::make_unique<Hooking>();
        featureInstance = std::make_unique<FeatureHandler>();

        hookingInstance->OriginalPresent = rendererInstance->GetPresent();

        Config::LoadConfig();

        if (IsDebugging)
            LOG_WARN("The build is built with the IsDebugging flag enabled.");

        if (SDK::Application_get_version && SDK::Application_get_unityVersion)
        {
            LOG_INFO("Game version: ", Utils::GetGameVersion());
            LOG_INFO("Unity version: ", Utils::GetUnityVersion());
        }
        else
            LOG_ERROR("Game version or untiy version not founded!");

        AHK(hookingInstance->OriginalPresent, Hooks::HkPresent); // ADD_HOOK
        //AHKA(Camera_get_main); // ADD_HOOK_AUTO

        hookingInstance->ApplyHooks();
        hooksApplied = true;

        NOTIFY_INFO_QUICK("Cheat injected successfully. The menu opens on " + Utils::getKeyName(MenuToggleKey));
        LOG_INFO("Cheat injected successfully. The menu opens on " + Utils::getKeyName(MenuToggleKey));

        while (CheatWork) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    catch (const std::exception& e) {
        LOG_ERROR(std::string("Exception in main thread: ") + e.what());
    }

    LOG_INFO("Starting cleanup...");

    try {
        Config::SaveConfig();

        if (rendererInstance) {
            ImGui::SaveIniSettingsToDisk((Utils::GetCheatDirectory() + "\\menu.ini").c_str());
        }
    }
    catch (...) {}

    if (hooksApplied && hookingInstance)
    {
        try {
            hookingInstance->RemoveHooks();
        }
        catch (...) {}
        hooksApplied = false;
    }

    hookingInstance.reset();
    rendererInstance.reset();
    featureInstance.reset();

    LOG_INFO("Cleanup completed");

finalize:
    loggerInstance.reset();

    FreeLibraryAndExitThread(globalModule, NULL);
    return 0;
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
    {
        CheatWork = true;
        DisableThreadLibraryCalls(hModule);
        globalModule = hModule;

        HANDLE hThread = CreateThread(nullptr, 0,
            reinterpret_cast<LPTHREAD_START_ROUTINE>(Il2CppBaseThread), // your cheat name
            nullptr, 0, nullptr);

        if (hThread)
            CloseHandle(hThread);

        break;
    }

    case DLL_PROCESS_DETACH:
    {
        CheatWork = false;
        Sleep(100);
        break;
    }
    }

    return TRUE;
}