#pragma once
#include "Cartridge.h"
#include "Graphics.h"

class MemoryBUS
{
public:
	MemoryBUS();
	~MemoryBUS();
	void init(const char* file, Cartridge* CartROM, Graphics* graphic);
	unsigned char read8b(unsigned short address);
	unsigned short read16b(unsigned short address);
	void write8b(unsigned short address, unsigned char value);
	void write16b(unsigned short address, unsigned short value);
private:
	int bank;
	Cartridge* ROM;
	Graphics* PPU;
	void LoadDMG(const char* file);
	unsigned char* DMGROM;
	bool startup;

};

