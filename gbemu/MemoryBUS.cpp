#include "MemoryBUS.h"
#include "stdafx.h"



MemoryBUS::MemoryBUS()
{
}


MemoryBUS::~MemoryBUS()
{
}

void MemoryBUS::init(Cartridge* CartROM, Graphics* graphic, Memory* memory)
{
	RAMbank = 0;
	ROMbank = 1;
	startup = true;
	ROM = CartROM;
	PPU = graphic;
	interrupt_Register = 0;
	ZeroMemory(&ZeroPage,sizeof(ZeroPage));
}

unsigned char MemoryBUS::read8b(unsigned short address)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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
		return ROM->read8bROM(ROMbank * 0x4000 + address);
		break;
	case 0x8000:
	case 0x9000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: PPU access(read) at 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
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
	case 0xA000:
	case 0xB000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Cartridge RAM access(read) at 0x" << std::hex << address << " Check MBC." << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		return ROM->read8bRAM(RAMbank * 0x2000+(address - 0xA000));
		break;

	case 0xF000:
		switch (address & 0xFF00)
		{
		case 0xFF00:
			if ((address >= 0xFF80)&&(address <0xFFFF))
			{
				return ZeroPage[address - 0xFF80];
			}			
			if(address == 0xFFFF)
				return interrupt_Register;			
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Illegal 8b read from pos: 0x" << std::hex << address << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			return 0;
			break;
		default:
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Illegal 8b read from pos: 0x" << std::hex << address << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			return 0;
		}
		break;
	default:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Illegal 8b read from pos: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		return 0;
	}
}

unsigned short MemoryBUS::read16b(unsigned short address)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	switch (address & 0xF000) {
	case 0x0000:			//Bank 0
	case 0x1000:
	case 0x2000:
	case 0x3000:
		if (address < 0x0100 && startup) {
			return DMGROM[address]+(DMGROM[address+1]<<8);
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
		return ROM->read16bROM(ROMbank * 0x4000 + address);
		break;
	case 0x8000:			//graphics
	case 0x9000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: PPU access(read) at 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
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
	case 0xA000:
	case 0xB000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Cartridge RAM access(read) at 0x" << std::hex << address << " Check MBC" << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		return ROM->read16bRAM(RAMbank * 0x2000 + (address - 0xA000));
		break;
	case 0xF000:
		switch (address & 0xFF00) {
		case 0xFF00:
			if (address >= 0xFF80) {
				return ZeroPage[address + 1 - 0xff80] + (ZeroPage[address - 0xff80] << 8);
			}
			break;
		default:
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Illegal 16b read from pos: 0x" << std::hex << address << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			return 0;
		}

	default:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Illegal 16b read from pos: 0x" << std::hex << address << std::endl;
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
		std::cout << "Error: Illegal 8b write to location: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x4000:			//switchable bank (mbc check needed)
	case 0x5000:
	case 0x6000:
	case 0x7000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Illegal 8b write to location: 0x" << std::hex << address << " Possibly bank switch. Check MBC" << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x8000:
	case 0x9000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: PPU access(write) at 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
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
	case 0xA000:
	case 0xB000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Cartridge RAM access(write) at 0x" << std::hex << address << " Check MBC" << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		ROM->write8bRAM(RAMbank * 0x2000 + (address - 0xA000), value);
		break;
	case 0xF000:
		switch (address & 0xFF00)
		{
		case 0xFF00:
			if (address < 0xff80)
			{
				SetConsoleTextAttribute(hConsole, 14);
				std::cout << "Illegal 8b write to loc: 0x" << std::hex << address << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
			}
			if ((address >= 0xff80) && (address < 0xffff))
				ZeroPage[address - 0xff80] = value;
			if (address == 0xFFFF)
				interrupt_Register = value;	
			break;
		default:
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Illegal 8b write to loc: 0x" << std::hex << address << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
		break;
	default:
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Error: Illegal 8b write to location: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
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
		std::cout << "Error: Illegal 16b write to location: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Illegal 16b write to location: 0x" << std::hex << address << " Possibly bank switch. Check MBC" << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		break;
	case 0x8000:
	case 0x9000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: PPU access(write) at 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
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
	case 0xA000:
	case 0xB000:
		SetConsoleTextAttribute(hConsole, 14);
		std::cout << "Warning: Cartridge RAM access(write) at 0x" << std::hex << address << " Check MBC" << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
		ROM->write16bRAM(RAMbank * 0x2000 + (address - 0xA000), value);
		break;
	case 0xF000:
		switch (address & 0xFF00) {
		case 0xFF00:
			if ((address < 0xff80) ||(address>=0xfffe))
			{
				SetConsoleTextAttribute(hConsole, 14);
				std::cout << "Illegal 16b write to loc: 0x" << std::hex << address << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
			}
			if ((address >= 0xff80) && (address < 0xfffe))
			{
				ZeroPage[address - 0xff80] = value & 0xFF;
				ZeroPage[address - 0xff80 + 1] = (value & 0xFF00) >> 8;
			}
			break;
		default:
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "Error: Illegal 16b write to location: 0x" << std::hex << address << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
		}
		break;
	default:
		SetConsoleTextAttribute(hConsole, 12);
		std::cout << "Error: Illegal 16b write to location: 0x" << std::hex << address << std::endl;
		SetConsoleTextAttribute(hConsole, 15);
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
