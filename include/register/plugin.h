#pragma once
#include <functional>
#include <string>

struct Plugin
{
    std::string name;
    std::function<void*()> factory;
    std::function<void(void*)> deleter;
};
