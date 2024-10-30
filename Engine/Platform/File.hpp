#pragma once

#include "Defines.hpp"
#include <string>
#include <vector>

#include <fstream>
#include <sstream>

enum class eFileMode {
	Read = 0x1,
	Write = 0x2
};

class File {
public:
	File() : IsValid(false){}
	File(const std::string& filename);
	virtual ~File() {}

public:
	std::string GetFilename() const { return Name; }
	std::string ReadBytes();
	bool WriteBytes(const char* source, size_t size, std::ios::openmode mode = std::ios::ate);
	bool IsExist();

private:
	std::string Name;
	bool IsValid;
};
