#include "IO.hpp"
#include "SwagDebug.hpp"

#include <format>
#include <iostream>

std::vector<char> IOHelper::readFile(const char* filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error(std::format("\nCould not open file {}!", filename));
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

#ifndef NDEBUG
	std::cout << "IO -> read file \"" << filename << "\"!" << "\n";
#endif // !NDEBUG


	return buffer;
}