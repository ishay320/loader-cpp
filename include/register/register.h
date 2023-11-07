#pragma once
#include "register/plugin_info.h"

template <typename PluginClass, typename... NoMoreInterfaces>
struct InterfaceHelper
{
   public:
    static void InsertInterfaces(PluginInfo::InterfaceCastingMap&)
    {
        // Do nothing. This is the terminal specialization of the variadic
        // template class member function.
    }
};

template <typename PluginClass, typename Interface, typename... RemainingInterfaces>
struct InterfaceHelper<PluginClass, Interface, RemainingInterfaces...>
{
   public:
    static void InsertInterfaces(PluginInfo::InterfaceCastingMap& interfaces)
    {
        // READ ME: If you get a compilation error here, then one of the
        // interfaces that you tried to register for your plugin is not
        // actually a base class of the plugin class. This is not allowed. A
        // plugin class must inherit every interface class that you want it to
        // provide.
        static_assert(std::is_base_of<Interface, PluginClass>::value,
                      "\033[33mYOU ARE ATTEMPTING TO REGISTER AN INTERFACE FOR A "
                      "PLUGIN, BUT THE INTERFACE IS NOT A BASE CLASS OF THE "
                      "PLUGIN.\033[0m");

        interfaces.insert(std::make_pair(typeid(Interface).name(),
                                         [=](void* v_ptr)
                                         {
                                             PluginClass* d_ptr = static_cast<PluginClass*>(v_ptr);
                                             return static_cast<Interface*>(d_ptr);
                                         }));

        InterfaceHelper<PluginClass, RemainingInterfaces...>::InsertInterfaces(interfaces);
    }
};

template <typename PluginClass, typename... Interfaces>
void filInfo(void* plugin_info)
{
    PluginInfo* plug = (PluginInfo*)plugin_info;
    plug->name       = typeid(PluginClass).name();
    plug->factory    = [=]() { return static_cast<void*>(new PluginClass); };
    plug->deleter    = [=](void* ptr) { delete static_cast<PluginClass*>(ptr); };
    InterfaceHelper<PluginClass, Interfaces...>::InsertInterfaces(plug->interfaces);
}

#define REGISTER_CLASS(...) \
    extern "C" void plugin(void* plugin_info) { filInfo<__VA_ARGS__>(plugin_info); }
