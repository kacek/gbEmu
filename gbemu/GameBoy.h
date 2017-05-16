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
	void init();
	void loadBIOS(char* filename);
	void loadROM(char* filename);
private:
	MemoryBUS* BUS;
	Cartridge* ROM;
	z80CPU* CPU;
	Graphics* PPU;
};

