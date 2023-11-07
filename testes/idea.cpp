#include <vector>

#include "interface.h"
#include "loader/loader.h"
#include "logger.h"

void passToCamera(CameraInterface* camera)
{
    LOG_INFO("------------- running function of camera ------------")
    camera->getImage();
    return;
}
void passToBase(BaseInterface* base)
{
    LOG_INFO("------------- running function of base ------------")
    base->start();
    base->stop();
    return;
}

int main(int argc, char const* argv[])
{
    if (argc < 2)
    {
        std::cout << "ERROR: no plugin name\n";
        std::cout << "Usage: " << argv[0] << " <plugin path>\n";
        return 1;
    }

    std::string path = argv[1];

    // load the wrapper for the loading
    Plugin plug{path, true};  // load + check if inhere from base + versions + construct

    if (!plug.loaded())
    {
        LOG_ERROR(plug.error());
        return 1;
    }

    // call the constructor of the plugin
    plug.construct();

    // get the base class - wrapper to getInterface
    BaseInterface* obj = plug.getBase();
    if (obj == nullptr)
    {
        LOG_ERROR("class error");
        return 1;
    }

    LOG_INFO("------------- running function of base ------------")
    obj->start();
    obj->stop();

    // do stuff on all interfaces
    std::vector<std::string> in = plug.getAllInterfaces();
    for (auto&& i : in)
    {
        if (typeid(CameraInterface).name() == i)
        {
            passToCamera(plug.getInterface<CameraInterface>());
        }
        if (typeid(BaseInterface).name() == i)
        {
            passToBase(plug.getInterface<BaseInterface>());
        }
    }

    LOG_INFO("------------ test for random interface ------------");
    std::string* p = plug.getInterface<std::string>();
    // check for error if you dont know that it exist
    if (p != nullptr)
    {
        std::cout << p->c_str() << '\n';
    }

    return 0;
}
