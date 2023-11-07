#pragma once
#include <memory>
#include <string>

class BaseInterface
{
   public:
    BaseInterface()      = default;
    ~BaseInterface()     = default;
    virtual bool start() = 0;
    virtual bool stop()  = 0;
};

class CameraInterface
{
   public:
    CameraInterface()                        = default;
    ~CameraInterface()                       = default;
    virtual std::shared_ptr<void> getImage() = 0;

   private:
    std::shared_ptr<void> frame;
};
