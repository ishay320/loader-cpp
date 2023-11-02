#pragma once
#include <iostream>

#define LOG_INFO(massage) std::cout << "\033[32m[INFO] " << __FILE__ << ':' << __LINE__ << " " << __FUNCTION__ << " \033[0m" << massage << "\n";
#define LOG_WARNING(massage) std::cout << "\033[33m[WARNING] " << __FILE__ << ':' << __LINE__ << " " << __FUNCTION__ << " \033[0m" << massage << "\n";
#define LOG_ERROR(massage) std::cerr << "\033[31m[ERROR] " << __FILE__ << ':' << __LINE__ << " " << __FUNCTION__ << " \033[0m" << massage << "\n";