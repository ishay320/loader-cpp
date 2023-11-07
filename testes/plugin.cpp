#include "interface.h"
#include "logger.h"
#include "register/register.h"

namespace test
{
class Bar : public BaseInterface
{
   public:
    Bar() { LOG_INFO("hello from class Bar"); }
    virtual ~Bar() { LOG_INFO("goodby from class Bar"); }

    void print(const std::string& host) { LOG_INFO("printed by request of '" << host << "'"); }

    virtual std::string name() { return "bar"; }
    virtual bool start()
    {
        LOG_INFO("hello from class Bar in function " << __FUNCTION__);
        return true;
    };
    virtual bool stop()
    {
        LOG_INFO("hello from class Bar in function " << __FUNCTION__);
        return true;
    };
    void function() { LOG_INFO("hello from class Bar in function"); }
};
}  // namespace test

REGISTER_CLASS(test::Bar, BaseInterface);
