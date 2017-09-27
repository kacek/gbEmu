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

void z80CPU::reset()
{
	a, b, c, d, e, h, l, f = 0;
	m, t = 0;
	PC, SP = 0;
	working = true;
	en_interr = false;
	en_interr_timer = 0;
	execute();
}

void z80CPU::execute()
{
	while (working)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		if (en_interr_timer > 0)
		{
			en_interr_timer--;
			if (en_interr_timer == 0) {
				en_interr = true;
				BUS->write8b(0xFFFF, 0xFF);
			}
		}
		op = BUS->read8b(PC);
		PC++;
		switch (op) {
		case 0x01:	//LD BC,d16 - load literal 16-bit value to register BC
			c = BUS->read8b(PC);
			b = BUS->read8b(PC + 1);
			m = 3;
			t = 12;
			PC += 2;
			break;
		case 0x0C:	//INC C - increment register C
			if ((c & 0xf) == 0xf)
			{
				f = f || 0x20;
			}
			if (c == 0xff)
			{
				c = 0x00;
				f |= 0x80;
			}
			else c++;
			f &= 0xBF;
			m = 1;
			t = 4;
			break;
		case 0x0E:	//LD C,d8 - load literal 8-bit value to register C
			c = BUS->read8b(PC);
			m = 2;
			t = 8;
			PC++;
			break;
		case 0x11:	//:D DE,d16 - load literal 16bits to register DE
			e = BUS->read8b(PC);
			d = BUS->read8b(PC + 1);
			PC += 2;
			m = 3;
			t = 12;
			break;
		case 0x1A:	//LD A,(DL) - load from address (DL) to A
			temp = (d << 8) + e;
			a = BUS->read8b(temp);
			m = 2;
			t = 8;
			break;
		case 0x20:	//JR NZ,r8 - relative jump if not zero
			if ((f & 0x80) != 0)
			{
				PC--;
				PC += BUS->read8b(PC + 1);
				m = 3;
				t = 12;
				SetConsoleTextAttribute(hConsole, 14);
				std::cout << "Warning: Jump to 0x" << std::hex << PC << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
			}
			else
			{
				m = 2;
				t = 8;
				PC++;
			}
			break;
		case 0x21:	//LD HL,d16 - load literal 16-bit value to register HL
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
		case 0x3E:	//LD A,d8 - load literal 8-bit value to register A
			a = BUS->read8b(PC);
			m = 2;
			t = 8;
			PC++;
			break;
		case 0x77: //LD (HL),A - store value from A in memory address (HL)
			temp = (h << 8) + l;
			BUS->write8b(temp, a);
			m = 2;
			t = 8;
			break;
		case 0xAF:	//XOR A - A XOR A, result stored in A
			a = a ^ a;
			a==0? f |= 0x80 : f &= 0x7F;
			m = 1;
			t = 4;
			break;
		case 0xCD:	//CALL a16 - push PC to stack, jump to address TODO: proper stack handling
			BUS->write16b(SP, PC + 1);
			PC = BUS->read16b(PC);
			SP--;
			m = 6;
			t = 24;
			break;
		case 0xE0:	//LDH (a8),A - put A in 0xFF00+a8
			BUS->write8b(0xFF00 + BUS->read8b(PC), a);
			PC++;
			m = 3;
			t = 12;
			break;
		case 0xE2:	//LD (C), A - store A in 0xFF00+C
			BUS->write8b(0xff00 + c, a);
			m = 2;
			t = 8;
			break;
		case 0xFB:	//EI - enable interrupts
			en_interr_timer = 2;
			m = 1;
			t = 4;
			break;
		case 0xCB:	//Prefix CB
			op = BUS->read8b(PC);
			PC++;
			switch (op)
			{
			case 0x7C:
				(h & 0x80) == 0 ? f |= 0x80 : f &= 0x7F;
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
			break;
		default:		
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "Warning: Unimplemented opcode 0x" << std::hex << (int)op  << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			working = false;
		}
	}
}
