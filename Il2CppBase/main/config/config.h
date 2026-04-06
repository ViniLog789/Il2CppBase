#pragma once
#include "Includes.h"

namespace Il2CppBase::Config // Your cheat name
{
    void LoadConfig();
    void SaveConfig();
    void LoadConfigFromFile(const std::string& filePath);
    void SaveConfigToFile(const std::string& filePath);
    void MergeConfigWithDefaults(nlohmann::json& existingConfig);

    struct BindConfig
    {
        std::string featureName;
        int key;
    };
}