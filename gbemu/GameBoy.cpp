#include "GameBoy.h"



GameBoy::GameBoy()
{
}


GameBoy::~GameBoy()
{
}

void GameBoy::init(const char* ROM_file, const char* DMG_file)
{
	ROM = new Cartridge();
	ROM->Load(ROM_file);
	PPU = new Graphics();
	BUS = new MemoryBUS();
	BUS->init(DMG_file,ROM, PPU);
	CPU = new z80CPU();
	CPU->init(BUS);
}
