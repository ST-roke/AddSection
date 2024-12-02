#ifndef ST_FILE_MANAGER_H
#define ST_FILE_MANAGER_H
#include <fstream>
#include <vector>

namespace FileManager
{
	auto IsExist(std::string filePath) -> bool;

	auto ReadBuffer(std::string filePath) -> std::vector<std::uint8_t>;
	void WriteBuffer(std::string filePath, std::vector<std::uint8_t> buffer);
}

#endif