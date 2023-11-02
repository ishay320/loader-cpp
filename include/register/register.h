#pragma once
#include "register/plugin.h"

#define REGISTER_CLASS(class)                               \
    void* factory() { return new class; }                   \
    void deleter(void* object) { delete ((class*)object); } \
                                                            \
    extern "C" void plugin(void* plugin)                    \
    {                                                       \
        Plugin* plug  = (Plugin*)plugin;                    \
        plug->name    = #class;                             \
        plug->factory = factory;                            \
        plug->deleter = deleter;                            \
    }
