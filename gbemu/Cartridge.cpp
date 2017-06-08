#include "Cartridge.h"
#include "stdafx.h"

Cartridge::Cartridge()
{
}


Cartridge::~Cartridge()
{
}

void Cartridge::Load(const char* file)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FILE* pFile;
	fopen_s(&pFile, file, "rb");
	if (pFile != NULL)
	{
		std::cout << "Loading ROM: " << file << std::endl;
		
		fseek(pFile, 0, SEEK_END);
		size = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		if (size != 0) {
			std::cout << "ROM size: " << size << " bytes" << std::endl;
			ROM = (unsigned char*)malloc(size*sizeof(unsigned char));
			if (fread(ROM, sizeof(char), size, pFile) != 0)
			{
				if (CheckLogo())
				{
					std::cout << "Logo check passed" << std::endl;
					std::cout << "Game ROM loaded successfully" << std::endl;
				}
				else
				{
					SetConsoleTextAttribute(hConsole, 12);
					std::cout << "Error: Logo check failed(might not be a valid GB ROM): " << file << std::endl;
					SetConsoleTextAttribute(hConsole, 15);
				}
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 12);
				std::cout << "Error: File read failed: " << file << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
			}
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "Error: File empty: " << file << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: File not found: " << file << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	}
	fclose(pFile);
}

void Cartridge::init()
{
	ZeroMemory(&RAM, sizeof(RAM));
}

bool Cartridge::CheckLogo()
{
	bool ok = true;
	for (int i = 0;i < 48;i++)
	{
		if (ROM[0x104 + i] != NintendoLogo[i]) ok = false;
	}
	return ok;
}

unsigned char Cartridge::read8bROM(unsigned short address)
{
	return ROM[address];
}

unsigned short Cartridge::read16bROM(unsigned short address)
{
	return ROM[address]+(ROM[address+1]<<8);
}

unsigned char Cartridge::read8bRAM(unsigned short address)
{
	return RAM[address];
}

unsigned short Cartridge::read16bRAM(unsigned short address)
{
	return RAM[address] + (RAM[address + 1] << 8);
}

void Cartridge::write8bRAM(unsigned short address, unsigned char value)
{
	RAM[address] = value;
}

void Cartridge::write16bRAM(unsigned short address, unsigned short value)
{
	RAM[address] = (value & 0xFF00);
	RAM[address + 1] = (value & 0x00FF) >> 8;
}
