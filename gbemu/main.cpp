#include "stdafx.h"
#include "GameBoy.h"

void main()
{
	GameBoy* App = new GameBoy();
	App->init("tetris.gb", "DMG_ROM.bin");


	system("pause");
}