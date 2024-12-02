#include "peImage.h"

auto PEImage::IsValid() -> bool
{
	PIMAGE_DOS_HEADER pDos = reinterpret_cast<PIMAGE_DOS_HEADER>(&imageBuffer[0]);
	if (pDos->e_magic != IMAGE_DOS_SIGNATURE)
		return false;

	PIMAGE_NT_HEADERS pNt = reinterpret_cast<PIMAGE_NT_HEADERS>(&imageBuffer[0] + pDos->e_lfanew);
	return pNt->Signature == IMAGE_NT_SIGNATURE;
}

void PEImage::AddSection(const char* sectionName, std::vector<std::uint8_t> sectionData)
{
	auto sectionSize = static_cast<std::uint32_t>(sectionData.size());

	PIMAGE_DOS_HEADER pDos = reinterpret_cast<PIMAGE_DOS_HEADER>(&imageBuffer[0]);
	PIMAGE_NT_HEADERS pNt = reinterpret_cast<PIMAGE_NT_HEADERS>(&imageBuffer[0] + pDos->e_lfanew);
	
	auto numberOfSections = pNt->FileHeader.NumberOfSections;
	PIMAGE_SECTION_HEADER sectionHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>(reinterpret_cast<uint8_t*>(pNt) + sizeof(IMAGE_NT_HEADERS));
	PIMAGE_SECTION_HEADER lastSection = &sectionHeader[numberOfSections - 1];
	PIMAGE_SECTION_HEADER newSection = &sectionHeader[numberOfSections];
	memset(newSection, 0, sizeof(IMAGE_SECTION_HEADER));//clear new section
	pNt->FileHeader.NumberOfSections++;

	memcpy(&newSection->Name, sectionName, strlen(sectionName));//set section name

	auto sectionAlignment = pNt->OptionalHeader.SectionAlignment;
	auto fileAlignment = pNt->OptionalHeader.FileAlignment;

	newSection->Misc.VirtualSize = ALIGN(sectionSize, sectionAlignment, 0);
	newSection->VirtualAddress = ALIGN(lastSection->Misc.VirtualSize, sectionAlignment, lastSection->VirtualAddress);
	newSection->SizeOfRawData = ALIGN(sectionSize, fileAlignment, 0);
	newSection->PointerToRawData = lastSection->PointerToRawData + lastSection->SizeOfRawData;
	newSection->Characteristics = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;

	pNt->OptionalHeader.SizeOfImage = newSection->VirtualAddress + newSection->Misc.VirtualSize;

	imageBuffer.insert(imageBuffer.end(), sectionData.begin(), sectionData.end());
}

void PEImage::CompileTo(std::string fileName)
{
	FileManager::WriteBuffer(fileName, imageBuffer);
}