#include "fileManager.h"

auto FileManager::IsExist(std::string filePath) -> bool
{
	std::ifstream file(filePath);
	return file.good();
}

auto FileManager::ReadBuffer(std::string filePath) -> std::vector<std::uint8_t>
{
	std::vector<std::uint8_t> buffer;
	std::ifstream file(filePath, std::ios::binary);
	if (!file)
		return buffer;

	file.seekg(0, std::ios::end);
	auto fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer.resize(fileSize);
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
	file.close();

	return buffer;
}

void FileManager::WriteBuffer(std::string filePath, std::vector<std::uint8_t> buffer)
{
	std::ofstream file(filePath, std::ios::binary);
	if (!file)
		return;

	file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
	file.close();
}