#pragma once

#include <string>
#include <sstream>
#include <iostream>

inline void StreamFormat(std::ostream& stream, const char* fmt) {
	stream << fmt;
}

template<typename...ArgsType, typename T>
void StreamFormat(std::ostream& stream, const char* fmt, const T& arg, const ArgsType&... args) {
	while (*fmt != 0) {
		if (*fmt == '%') {
			stream << arg;
			return StreamFormat(stream, fmt + 1, args...);
		}
		stream << *fmt;
		++fmt;
	}
}

template<typename...ArgsType>
std::string Format(const char* fmt, const ArgsType&... args) {
	std::stringstream stream;
	StreamFormat(stream, fmt, args...);
	return stream.str();
}

template<typename...ArgsType>
int Error(const char* fmt, const ArgsType&...args) {
	StreamFormat(std::cerr, fmt, args...);
	std::cerr << '\n';

	return EXIT_FAILURE;
}

template<typename...ArgsType>
void Println(const char* fmt, const ArgsType&...args) {
	StreamFormat(std::cout, fmt, args...);
	std::cout << '\n';
}
