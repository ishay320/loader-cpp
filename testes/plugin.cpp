#include <memory>

#include "interface.h"
#include "logger.h"
#include "register/register.h"

namespace test
{
class Bar : public Plug
{
   public:
    Bar() { LOG_INFO("hello from class in plug"); }
    ~Bar() { LOG_INFO("goodby from class in plug"); }

    void print(const std::string& host) { LOG_INFO("printed by request of '" << host << "'"); }

    virtual std::string name() { return "bar"; }
    virtual bool start() { return true; };
    virtual bool stop() { return true; };
    void function() { LOG_INFO("hello from class in plug in function"); }
};
}  // namespace test

REGISTER_CLASS(test::Bar);
