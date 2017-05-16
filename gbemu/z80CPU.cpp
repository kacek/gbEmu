#include "z80CPU.h"



z80CPU::z80CPU()
{
}


z80CPU::~z80CPU()
{
}

void z80CPU::init(MemoryBUS * memBUS)
{
	BUS = memBUS;
}
