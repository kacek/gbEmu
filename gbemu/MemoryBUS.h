#pragma once
#include "Cartridge.h"
#include "Graphics.h"
#include "Memory.h"

class MemoryBUS
{
public:
	MemoryBUS();
	~MemoryBUS();
	void init(Cartridge* CartROM, Graphics* graphic, Memory* memory);
	unsigned char read8b(unsigned short address);
	unsigned short read16b(unsigned short address);
	void write8b(unsigned short address, unsigned char value);
	void write16b(unsigned short address, unsigned short value);
	void LoadDMG(const char* file);
private:
	int ROMbank;
	int RAMbank;
	char ZeroPage[127];
	Memory* RAM;
	Cartridge* ROM;
	Graphics* PPU;
	unsigned char* DMGROM;
	bool startup;
	char interrupt_Register;
};

