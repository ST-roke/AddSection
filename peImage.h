#ifndef ST_PE_IMAGE_H
#define ST_PE_IMAGE_H
#include "fileManager.h"
#include <Windows.h>
#include <string>
#include <vector>
#define ALIGN(size, align, addr) ((size % align) ? (addr + (size / align + 1) * align) : (addr + size))

class PEImage
{
private:
	std::vector<std::uint8_t> imageBuffer;

public:
	explicit PEImage(std::vector<std::uint8_t>& peImage) : imageBuffer(std::move(peImage)) {}

	auto IsValid() -> bool;

	void AddSection(const char* sectionName, std::vector<std::uint8_t> sectionData);

	void CompileTo(std::string fileName);
};

#endif