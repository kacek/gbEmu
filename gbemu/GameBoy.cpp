#include "GameBoy.h"



GameBoy::GameBoy()
{
}


GameBoy::~GameBoy()
{
}

void GameBoy::init()
{
	ROM = new Cartridge();
	PPU = new Graphics();
	BUS = new MemoryBUS();
	CPU = new z80CPU();
	CPU->init(BUS);
	ROM->init();
}

void GameBoy::loadBIOS(char * filename)
{
	BUS->LoadDMG(filename);
}

void GameBoy::loadROM(char * filename)
{
	ROM->Load(filename);
}
