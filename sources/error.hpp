#pragma once

#include <iostream>
#include <core/format.hpp>

template<typename...ArgsType>
int Error(const char* fmt, const ArgsType&...args) {
	std::cerr << Format(fmt, args...);

	return EXIT_FAILURE;
}

