#include "loader/loader.h"

#include <dlfcn.h>

#include "interface.h"
#include "logger.h"
#include "register/plugin.h"

std::shared_ptr<void> loadLib(const std::string& full_path, bool no_delete)
{
    std::shared_ptr<void> dlHandlePtr;
    dlerror();  // Call to clear the error if one exist

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
        LOG_ERROR("Error while loading the library '" << full_path << "': " << load_error);

        // Just return a nullptr if the library could not be loaded.
        return nullptr;
    }

    return std::shared_ptr<void>(dl_handle, [](void* ptr) { dlclose(ptr); });
}

std::shared_ptr<Plug> loadPlug(std::shared_ptr<void> lib_handle)
{
    // Load the function from the shared object
    std::string symbol = "plugin";
    typedef void (*factory_function)(void*);
    factory_function foo = (factory_function)dlsym(lib_handle.get(), symbol.c_str());

    const char* dlsym_error = dlerror();
    if (dlsym_error)
    {
        LOG_ERROR("Error loading symbol 'foo': " << dlsym_error);
        return nullptr;
    }

    // Call the "foo" function from the shared object
    Plugin plug;
    foo((void*)&plug);

    return std::shared_ptr<Plug>{(Plug*)plug.factory(), plug.deleter};
}