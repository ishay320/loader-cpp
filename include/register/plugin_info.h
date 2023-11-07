#pragma once
#include <functional>
#include <string>
#include <unordered_map>

struct PluginInfo
{
    std::string name;
    std::function<void*()> factory;
    std::function<void(void*)> deleter;

    using InterfaceCastingMap = std::unordered_map<std::string, std::function<void*(void*)> >;
    InterfaceCastingMap interfaces;
};
// TODO: versions
// TODO: maybe add alignment checking and size