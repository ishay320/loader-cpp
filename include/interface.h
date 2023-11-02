#pragma once
#include <string>

class Plug
{
   public:
    Plug()  = default;
    ~Plug() = default;

    virtual void print(const std::string &host) = 0;
    virtual bool start()                        = 0;
    virtual bool stop()                         = 0;

    virtual std::string name() = 0;
};