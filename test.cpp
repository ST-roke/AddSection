#include "peImage.h"
#include <iostream>
#include <memory>

int main()
{
_getFilePath:
	std::string filePath;
	std::cout << "[+] File Path: ";
	std::cin >> filePath;

	if (!FileManager::IsExist(filePath))
	{
		std::cout << "[-] File does not exist." << std::endl;
		goto _getFilePath;
	}

	auto fileBuffer = FileManager::ReadBuffer(filePath);
	if (fileBuffer.empty())
	{
		std::cout << "[-] The size of file is 0." << std::endl;
		goto _getFilePath;
	}

	auto pe = std::make_shared<PEImage>(fileBuffer);
	if (!pe->IsValid())
	{
		std::cout << "[-] The PE of file is invalid." << std::endl;
		goto _getFilePath;
	}

	std::vector<std::uint8_t> testSection(0x1000, 0x00);
	pe->AddSection(".test1", testSection);

	auto newFilePath = filePath;
	size_t pos = newFilePath.rfind(".exe");
	if (pos != std::string::npos) 
	{
		newFilePath.replace(pos, 4, "_new.exe");
	}

	pe->CompileTo(newFilePath);
	std::cout << "[+] Build: " << newFilePath << std::endl;
	system("pause");
	return 0;
}