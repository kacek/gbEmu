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
		case 0x1:	//LD BC,d16 - load literal 16-bit value to register BC
			c = BUS->read8b(PC);
			b = BUS->read8b(PC + 1);
			m = 3;
			t = 12;
			PC += 2;
			break;
		case 0x31:	//LD SP, d16 - set stack pointer to literal value
			SP = BUS->read16b(PC);
			m = 3;
			t = 12;
			PC += 2;
			break;
		case 0xAF:	//XOR A - A XOR A, result stored in A
			a = a ^ a;
			m = 1;
			t = 4;
			break;
		default:
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Warning: Unimplemented opcode 0x" << std::hex << (int)op  << std::endl;
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
