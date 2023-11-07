#include "loader/loader.h"

#include <dlfcn.h>

#include "interface.h"
#include "logger.h"
#include "register/plugin_info.h"

Plugin::Plugin(const std::string& dll_path, bool no_delete)
{
    // load the file
    _handle = loadLib(dll_path, no_delete);
    if (_handle == nullptr)
    {
        return;
    }

    // load the function from the file
    factory_function factory = loadSymbol();
    if (factory == nullptr)
    {
        return;
    }

    // load the info from the function
    _plug = loadPluginInfo(factory);
}

Plugin::~Plugin() {}

bool Plugin::loaded() { return _handle != nullptr; }

const std::string& Plugin::error() { return _error; }

std::vector<std::string> Plugin::getAllInterfaces()
{
    std::vector<std::string> interfaces;
    for (const auto& pair : _plug.interfaces)
    {
        interfaces.push_back(pair.first);
    }
    return interfaces;
}

BaseInterface* Plugin::getBase() { return getInterface<BaseInterface>(); }

std::shared_ptr<void> Plugin::construct()
{
    // construct the main class
    _object = std::shared_ptr<void>{_plug.factory(), _plug.deleter};
    return _object;
}

std::shared_ptr<void> Plugin::loadLib(const std::string& full_path, bool no_delete)
{
    std::shared_ptr<void> dlHandlePtr;
    dlerror();  // Call to clear the error if one exists

    // NOTE: We open using RTLD_LOCAL instead of RTLD_GLOBAL to prevent the
    // symbols of different libraries from writing over each other.
#ifdef _WIN32
                // RTLD_NODELETE is not defined in dlfcn-32.
    (void)no_delete;
    int dlopenMode = RTLD_LAZY | RTLD_LOCAL;
#else
    int dl_open_mode = RTLD_LAZY | RTLD_LOCAL | (no_delete ? RTLD_NODELETE : 0);
#endif
    void* dl_handle = dlopen(full_path.c_str(), dl_open_mode);

    const char* load_error = dlerror();
    if (nullptr == dl_handle || nullptr != load_error)
    {
        LOG_ERROR("Error while loading the library: " << load_error);
        _error = "Error while loading the library: " + std::string(load_error);
        // Just return a nullptr if the library could not be loaded.
        return nullptr;
    }

    return std::shared_ptr<void>(dl_handle, [](void* ptr) { dlclose(ptr); });
}

using factory_function = void (*)(void*);
factory_function Plugin::loadSymbol(const std::string& symbol)
{
    // Load the function from the shared object
    factory_function factory = (factory_function)dlsym(_handle.get(), symbol.c_str());

    const char* dlsym_error = dlerror();
    if (dlsym_error)
    {
        LOG_ERROR("Error loading symbol '" << symbol << "' " << dlsym_error);
        _error = "Error loading symbol '" + symbol + "' " + dlsym_error;
        return nullptr;
    }
    return factory;
}

PluginInfo Plugin::loadPluginInfo(factory_function factory)
{
    // Call the "factory" function to get the dll data
    PluginInfo plug;
    factory((void*)&plug);
    return plug;
}

// TODO: remove the logging