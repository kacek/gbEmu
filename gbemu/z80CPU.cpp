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
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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
		case 0x21:	//LD HL,d16 - load literal 16b-bit value to register HL
			l = BUS->read8b(PC);
			h = BUS->read8b(PC + 1);
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
		case 0x32:	//LD (HL-),A - put value from A at address (HL), decrement HL
			temp = (h << 8) + l;
			BUS->write8b(temp, a);
			temp--;
			h = (temp & 0xFF00) >> 8;
			l = temp & 0x00FF;
			m = 2;
			t = 8;
			break;
		case 0xAF:	//XOR A - A XOR A, result stored in A
			a = a ^ a;
			a==0? f |= 0x80 : f &= 0x7F;
			m = 1;
			t = 4;
			break;
		case 0xCB:	//Prefix CB
			op = BUS->read8b(PC);
			PC++;
			switch (op)
			{
			case 0x7C:
				h & 0x80 == 0 ? f |= 0x80 : f &= 0x7F;
				f &= 0xBF;
				f |= 0x20;
				break;
			default:
				SetConsoleTextAttribute(hConsole, 14);
				std::cout << "Warning: Unimplemented opcode 0xCB" << std::hex << (int)op << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
				working = false;
			}
			m = 2;
			h = 8;
			PC++;
			break;
		default:		
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
