#pragma once

#include <string>
#include <vector>

#include "interface.h"
#include "logger.h"
#include "register/plugin_info.h"

class Plugin
{
   public:
    Plugin(const std::string& dll_path, bool no_delete = true);

    ~Plugin();

    bool loaded();

    const std::string& error();

    template <class Interface>
    Interface* getInterface()
    {
        if (_object == nullptr)
        {
            LOG_ERROR("class was not constructed");
            return nullptr;
        }

        const auto& it = _plug.interfaces.find(typeid(Interface).name());
        if (_plug.interfaces.end() == it)
        {
            LOG_ERROR("class not implementing interface: '" << typeid(Interface).name() << "'");
            return nullptr;
        }

        return static_cast<Interface*>(_object.get());
    }

    std::vector<std::string> getAllInterfaces();

    BaseInterface* getBase();

    std::shared_ptr<void> construct();

   private:
    PluginInfo _plug;

    std::shared_ptr<void> _handle; /* handle of the dll file */
    std::shared_ptr<void> _object; /* pointer to the class and the destructor */

    std::string _error;

    std::shared_ptr<void> loadLib(const std::string& full_path, bool no_delete);

    using factory_function = void (*)(void*);
    factory_function loadSymbol(const std::string& symbol = "plugin");
    PluginInfo loadPluginInfo(factory_function factory);
};