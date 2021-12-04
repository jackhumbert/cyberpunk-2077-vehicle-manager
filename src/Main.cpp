#include <iostream>

#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/IScriptable.hpp>
#include <RED4ext/RTTITypes.hpp>
#include <RED4ext/CString.hpp>
#include <RED4ext/InstanceType.hpp>
#include <RED4ext/Scripting/Natives/Generated/game/VehicleSystem.hpp>

#include "Utils.hpp"
#include "stdafx.hpp"
#include "TweakDBWriter.hpp"

std::vector<std::string> garageVehicles = std::vector<std::string>();

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes()
{
    spdlog::info("Registering classes & types");
}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes()
{
    spdlog::info("Registering functions");
}

YAML::Node LoadConfig(std::string path) {
    auto configFilePath = Utils::GetRootDir() / path;
    YAML::Node config;
    try
    {
        if (!std::filesystem::exists(configFilePath))
        {
            std::string fileContents = "";
            std::ofstream fw(configFilePath, std::ofstream::out);
            if (!fw.is_open())
                spdlog::error("Could not create the config file");
            fw << fileContents;
            fw.close();
        }
        config = YAML::LoadFile(configFilePath.string().c_str());
    }
    catch (const std::exception& ex)
    {
        spdlog::error("An exception occured while reading the config file");
        spdlog::error(ex.what());
    }
    catch (...)
    {
        spdlog::error("An unknown error occured while reading the config file");
    }
    return config;
}

std::vector<std::string> FindVehicles(std::string path) {
    std::vector<std::string> vehicles = std::vector<std::string>();

    auto vehicleDirectory = Utils::GetRootDir() / path;
    try
    {
        if (!std::filesystem::exists(vehicleDirectory))
        {
            std::filesystem::create_directories(vehicleDirectory);
        }

        for (const auto& vehicleTweak : std::filesystem::recursive_directory_iterator(vehicleDirectory))
        {
            const auto& path = vehicleTweak.path();
            if (path.extension() == ".bin")
            {
                spdlog::info("Found {}", path.filename().string());
                vehicles.push_back(path.filename().stem().string());
            }
        }
    }
    catch (const std::exception& ex)
    {
        spdlog::error("An exception occured while reading the directory");
        spdlog::error(ex.what());
    }
    catch (...)
    {
        spdlog::error("An unknown error occured while reading the directory");
    }
    return vehicles;
}

BOOL APIENTRY DllMain(HMODULE aModule, DWORD aReason, LPVOID aReserved)
{
    switch (aReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(aModule);
        //RED4ext::RTTIRegistrator::Add(RegisterTypes, PostRegisterTypes);

        Utils::CreateLogger();
        spdlog::info("Starting up");

        if (auto config = LoadConfig("r6/vehicle_manager.yaml")) {
            auto vehicles = FindVehicles("r6/tweakdbs/vehicles");
            for (auto vehicle : vehicles) {
                if (YAML::Node vehicleConfig = config["vehicles"][vehicle]) {
                    spdlog::info("Found configuration: {}", vehicle);
                    if (vehicleConfig["addToGarage"]) {
                        spdlog::info("Configured to add to garage: {}", vehicle);
                        garageVehicles.push_back(vehicle);
                    }
                }
            }
            auto vehicleManagerTweakDirectory = Utils::GetRootDir() / "r6/tweakdbs/vehicle_manager";
            auto vehicleListFile = vehicleManagerTweakDirectory / "vehicle_list.bin";
            if (!std::filesystem::exists(vehicleManagerTweakDirectory))
            {
                std::filesystem::create_directories(vehicleManagerTweakDirectory);
            }

            writeTweakDB(vehicles, vehicleListFile);
        }

        break;
    }
    case DLL_PROCESS_DETACH:
    {
        spdlog::info("Shutting down");
        spdlog::shutdown();

        break;
    }
    }

    return TRUE;
}

RED4EXT_C_EXPORT bool RED4EXT_CALL Load(RED4ext::PluginHandle aHandle, const RED4ext::IRED4ext* aInterface)
{
    //spdlog::info("Attempting to add vehicles to garage");

    //RED4ext::ScriptGameInstance gameInstance;
    //RED4ext::Handle<RED4ext::IScriptable> vs;
    //RED4ext::ExecuteGlobalFunction("GetVehicleSystem;GameInstance", &vs, gameInstance);

    //if (vs) {
    //    auto rtti = RED4ext::CRTTISystem::Get();
    //    auto gameVehicleSystemCls = rtti->GetClass("gameVehicleSystem");
    //    auto enablePlayerVehicleFunc = gameVehicleSystemCls->GetFunction("EnablePlayerVehicle");

    //    for (std::string vehicle : garageVehicles) {
    //        spdlog::info("Adding to garage: {}", vehicle);

    //        RED4ext::CString vehicleName = vehicle.c_str();
    //        bool enable = true;
    //        bool despawn = false;

    //        RED4ext::StackArgs_t args;
    //        args.emplace_back(nullptr, &vehicleName);
    //        args.emplace_back(nullptr, &enable);
    //        args.emplace_back(nullptr, &despawn);

    //        bool returnValue;
    //        RED4ext::ExecuteFunction(vs, enablePlayerVehicleFunc, &returnValue, args);
    //        if (returnValue) {
    //            spdlog::info("SUCCESS");
    //        } else {
    //            spdlog::error("ERROR");
    //        }
    //    }
    //}
    //else
    //{
    //    spdlog::error("Something in RTTI unavailable");
    //}

    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Unload()
{
    
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo* aInfo)
{
    aInfo->name = L"Vehicle Manager";
    aInfo->author = L"Jack Humbert";
    aInfo->version = RED4EXT_SEMVER(0, 0, 1);
    aInfo->runtime = RED4EXT_RUNTIME_LATEST;
    aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports()
{
    return RED4EXT_API_VERSION_LATEST;
}
