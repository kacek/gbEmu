#include "MemoryBUS.h"
#include "stdafx.h"



MemoryBUS::MemoryBUS()
{
}


MemoryBUS::~MemoryBUS()
{
}

void MemoryBUS::init(Cartridge* CartROM, Graphics* graphic)
{
	bank = 1;
	startup = true;
	ROM = CartROM;
	PPU = graphic;
}

unsigned char MemoryBUS::read8b(unsigned short address)
{
	switch(address & 0xF000) {
	case 0x0000:		//Bank 0
	case 0x1000:
	case 0x2000:
	case 0x3000:
		if (address < 0x0100 && startup) {
			return DMGROM[address];
		}
		else
		{
			return ROM->read8bROM(address);
		}
		break;
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return ROM->read8bROM(bank * 0x4000 + address);
		break;
	case 0x8000:
	case 0x9000:
		if (address < 0x9800)
		{
			return PPU->read8bTile(address - 0x8000);
		}else if (address < 0x9C00)
		{
			return PPU->read8bBG1(address - 0x9800);
		}
		else {
			return PPU->read8bBG2(address - 0x9C00);
		}
		break;

	default:
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Illegal read from pos: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		return 0;
	}
}

unsigned short MemoryBUS::read16b(unsigned short address)
{
	switch (address & 0xF000) {
	case 0x0000:			//Bank 0
	case 0x1000:
	case 0x2000:
	case 0x3000:
		if (address < 0x0100 && startup) {
			return (DMGROM[address]<<8)+DMGROM[address+1];
		}
		else
		{
			return ROM->read16bROM(address);
		}
		break;
	case 0x4000:			//switchable bank
	case 0x5000:
	case 0x6000:
	case 0x7000:
		return ROM->read16bROM(bank * 0x4000 + address);
		break;
	case 0x8000:			//graphics
	case 0x9000:
		if (address < 0x9800)
		{
			return PPU->read16bTile(address - 0x8000);
		}
		else if (address < 0x9C00)
		{
			return PPU->read16bBG1(address - 0x9800);
		}
		else {
			return PPU->read16bBG2(address - 0x9C00);
		}
		break;

	default:
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Illegal read from pos: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		return 0;
	}
}

void MemoryBUS::write8b(unsigned short address, unsigned char value)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (address & 0xF000) {
	case 0x0000:			//Bank 0
	case 0x1000:
	case 0x2000:
	case 0x3000:
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Error: Illegal write to location: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x4000:			//switchable bank (mbc check needed)
	case 0x5000:
	case 0x6000:
	case 0x7000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Illegal write to location: 0x" << std::hex << address << " Possibly bank switch. Check MBC" << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x8000:
	case 0x9000:
		if (address < 0x9800)
		{
			PPU->write8bTile(address - 0x8000,value);
		}
		else if (address < 0x9C00)
		{
			PPU->write8bBG1(address - 0x9800, value);
		}
		else {
			PPU->write8bBG2(address - 0x9C00, value);
		}
		break;
	}
}

void MemoryBUS::write16b(unsigned short address, unsigned short value)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (address & 0xF000) {
	case 0x0000:			//Bank 0
	case 0x1000:
	case 0x2000:
	case 0x3000:
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Error: Illegal write to location: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Illegal write to location: 0x" << std::hex << address << " Possibly bank switch. Check MBC" << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x8000:
	case 0x9000:
		if (address < 0x9800)
		{
			PPU->write16bTile(address - 0x8000, value);
		}
		else if (address < 0x9C00)
		{
			PPU->write16bBG1(address - 0x9800, value);
		}
		else {
			PPU->write16bBG2(address - 0x9C00, value);
		}
		break;
	}
}

void MemoryBUS::LoadDMG(const char* file)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FILE* pFile;
	fopen_s(&pFile, file, "rb");
	if (pFile != NULL) {
		fseek(pFile, 0, SEEK_END);
		int size = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		if (size == 256)
		{
			DMGROM = (unsigned char*)malloc(size*sizeof(unsigned char));
			if (fread(DMGROM, sizeof(char), size, pFile) != 0)
			{
				std::cout << "DMG ROM loaded successfully" << std::endl;
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 12);
				std::cout << "Error: DMG ROM File read failed: " << file << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
			}
		}
		else
		{
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "Error: DMG File size not match: " << file << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: DMG File not found: " << file << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
	}
}
