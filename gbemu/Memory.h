#pragma once
#include "stdafx.h"
class Memory
{
public:
	Memory();
	~Memory();
	void init();
	unsigned char read8bRAM(unsigned short address);
	unsigned short read16bRAM(unsigned short address);
	void write8bRAM(unsigned short address, unsigned char value);
	void write16bRAM(unsigned short address, unsigned short value);

private:
	unsigned char RAM[0x2000];
};

