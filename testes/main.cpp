#include "interface.h"
#include "loader/loader.h"
#include "logger.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        std::cout << "ERROR: no plugin name\n";
        std::cout << "Usage: " << argv[0] << " <plugin path>\n";
        return 1;
    }

    std::string path                 = argv[1];
    std::shared_ptr<void> lib_handle = loadLib(path, true);

    if (nullptr == lib_handle)
    {
        LOG_ERROR("could not load the library '" << path << "'");
        return 1;
    }
    std::shared_ptr<Plug> plug = loadPlug(lib_handle);
    plug->print("loader");

    LOG_INFO("the loaded plugin name is '" << plug->name() << "'");

    LOG_INFO("hello from loader");
}