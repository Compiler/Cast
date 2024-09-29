#pragma once

#include <fmt/core.h>

#define _RESET "\033[0m"
#define _GREEN "\033[32m"
#define _BLUE "\033[34m"
#define CAST_DEBUG(fmt_str, ...) std::cout << _GREEN << "[" << __FILE__ << ":" << __LINE__ << "] " << fmt::format(fmt_str, __VA_ARGS__) << _RESET << std::endl;
#define CAST_LOG(fmt_str, ...) std::cout << _BLUE << "[" << __FILE__ << ":" << __LINE__ << "] " << fmt::format(fmt_str, __VA_ARGS__) << _RESET << std::endl;

