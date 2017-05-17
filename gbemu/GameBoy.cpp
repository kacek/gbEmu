#include "GameBoy.h"



GameBoy::GameBoy()
{
}


GameBoy::~GameBoy()
{
}

void GameBoy::init()
{
	RAM = new Memory();
	ROM = new Cartridge();
	PPU = new Graphics();
	BUS = new MemoryBUS();
	CPU = new z80CPU();
	CPU->init(BUS);
	BUS->init(ROM, PPU, RAM);
	ROM->init();
	RAM->init();
}

void GameBoy::loadBIOS(char * filename)
{
	BUS->LoadDMG(filename);
}

void GameBoy::loadROM(char * filename)
{
	ROM->Load(filename);
}
