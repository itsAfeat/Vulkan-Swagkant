#ifndef IO_H
#define IO_H

#include <fstream>
#include <vector>

class IOHelper {
public:
	static std::vector<char> readFile(const char* filename);
};

#endif // !IO_H
