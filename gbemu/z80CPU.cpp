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

void z80CPU::execute()
{
	while (working)
	{
		op = BUS->read8b(PC);
		PC++;
		switch (op) {
		default:
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Warning: Unimplemented opcode 0x" << std::hex << op  << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			working = false;
		}
	}
}

void z80CPU::reset()
{
	a, b, c, d, e, h, l, f = 0;
	m, t = 0;
	PC, SP = 0;
	working = true;
	execute();
}
