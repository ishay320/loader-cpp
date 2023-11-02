#pragma once
#include <memory>
#include <string>

#include "interface.h"

std::shared_ptr<void> loadLib(const std::string& full_path, bool no_delete);

std::shared_ptr<Plug> loadPlug(std::shared_ptr<void> lib_handle);