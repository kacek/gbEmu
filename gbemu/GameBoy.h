#pragma once
#include "Cartridge.h"
#include "MemoryBUS.h"
#include "z80CPU.h"
#include "Graphics.h"
class GameBoy
{
public:
	GameBoy();
	~GameBoy();
	void init(const char* ROM_file, const char* DMG_file);
private:
	MemoryBUS* BUS;
	Cartridge* ROM;
	z80CPU* CPU;
	Graphics* PPU;
};

