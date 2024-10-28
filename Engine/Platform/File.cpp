#include "File.hpp"
#include "FileSystem.hpp"
#include "Core/EngineLogger.hpp"

#include <string>

File::File(const std::string& fn) {
	Name = fn;
}

std::string File::ReadBytes() {
	std::stringstream buffer;
	std::ifstream inFile(Name); // ���ļ�

	if (!inFile) { // ����ļ��Ƿ�ɹ���
		std::cerr << "�޷����ļ�!" << std::endl;
		return "";
	}

	std::string line;
	while (std::getline(inFile, line)) { 
		buffer << line << std::endl;
	}

	inFile.close();

	return std::string(buffer.str());
}

bool File::WriteBytes(const char* source, size_t size, std::ios::openmode mode) {
	std::ofstream outFile(Name, mode);

	if (!outFile) { 
		std::cerr << "�޷����ļ�!" << std::endl;
		return false; 
	}

	// ���ļ�д������
	outFile.write(source, size);
	outFile.close(); // �ر��ļ�

	return true;
}

bool File::IsExist() {
#ifdef _MSC_VER
	struct _stat buffer;
	return _stat(Name.c_str(), &buffer) == 0;
#else
	struct stat buffer;
	return stat(Name.c_str(), &buffer) == 0;
#endif
}
