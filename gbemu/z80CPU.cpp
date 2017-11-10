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
		switch (op) {
		case 0x01:	//LD BC,d16 - load literal 16-bit value to register BC
			c = BUS->read8b(PC + 1);
			b = BUS->read8b(PC + 2);
			m = 3;
			t = 12;
			PC += 2;
			break;
		case 0x05:	//DEC B - decrease register B
			(b & 0xf) == 0 ? f &= 0xdf : f |= 0x20;
			b--;
			b == 0 ? f |= 0x80 : f &= 0x7F;
			f |= 0x40;
			m = 1;
			t = 4;
			break;
		case 0x06:	//LD B, d8 - load literal 8bit value into register B
			b = BUS->read8b(PC+1);
			PC++;
			m = 2;
			t = 8;
			break;
		case 0x0C:	//INC C - increment register C
			(c & 0xf) == 0xf ? f |= 0x20 : f &= 0xdf;
			c == 0xff? f |= 0x80 : f &= 0x7f;
			c++;
			f &= 0xBF;
			m = 1;
			t = 4;
			break;
		case 0x0E:	//LD C,d8 - load literal 8-bit value to register C
			c = BUS->read8b(PC+1);
			m = 2;
			t = 8;
			PC++;
			break;
		case 0x11:	//LD DE,d16 - load literal 16bits to register DE
			e = BUS->read8b(PC+1);
			d = BUS->read8b(PC + 2);
			PC += 2;
			m = 3;
			t = 12;
			break;
		case 0x17:	//RLA - rotate A left through carry
			temp = a;
			a <<= 1;
			a += (f & 0x10) >> 4;
			(temp & 0x80) != 0 ? f |= 0x10 : f &= 0xef;
			f &= 0x1F;
			m = 1;
			t = 4;
			break;
		case 0x1A:	//LD A,(DE) - load from address (DE) to A
			temp = (d << 8) + e;
			a = BUS->read8b(temp);
			m = 2;
			t = 8;
			break;
		case 0x20:	//JR NZ,r8 - relative jump if not zero
			if ((f & 0x80) == 0)
			{
				PC += (signed char)BUS->read8b(PC + 1) + 1;
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
			l = BUS->read8b(PC + 1);
			h = BUS->read8b(PC + 2);
			m = 3;
			t = 12;
			PC += 2;
			break;
		case 0x22:	//LD (HL+),A - put value from A to address (HL), increment HL
			temp = (h << 8) + l;
			BUS->write8b(temp, a);
			temp++;
			h = (temp & 0xFF00) >> 8;
			l = temp & 0xff;
			m = 2;
			t = 8;
			break;
		case 0x31:	//LD SP, d16 - set stack pointer to literal value
			SP = BUS->read16b(PC + 1);
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
			a = BUS->read8b(PC + 1);
			m = 2;
			t = 8;
			PC++;
			break;
		case 0x4F:	//LD C,A - put value from register A to register C
			c = a;
			m = 1;
			t = 4;
			break;
		case 0x77: //LD (HL),A - store value from A in memory address (HL)
			temp = (h << 8) + l;
			BUS->write8b(temp, a);
			m = 2;
			t = 8;
			break;
		case 0xAF:	//XOR A - A XOR A, result stored in A
			a ^= a;
			a==0? f |= 0x80 : f &= 0x7F;
			f &= 0x8F;
			m = 1;
			t = 4;
			break;
		case 0xC1:	//POP BC - load value to register BC from stack
			temp = BUS->read16b(SP);
			b = (temp & 0xFF00) >> 8;
			c = temp & 0x00FF;
			m = 3;
			t = 12;
			break;
		case 0xC5: //PUSH BC - put value from BC register on stack
			BUS->write16b(SP - 1, ((b << 8) + c));
			SP -= 2;
			m = 4;
			t = 16;
			break;
		case 0xCD:	//CALL a16 - push PC to stack, jump to address TODO: proper stack handling
			BUS->write16b(SP-1, PC + 2);
			PC = BUS->read16b(PC + 1);
			SP -= 2;
			m = 6;
			t = 24;
			break;
		case 0xE0:	//LDH (a8),A - put A in 0xFF00+a8
			BUS->write8b(0xFF00 + BUS->read8b(PC+1), a);
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
			op = BUS->read8b(PC+1);
			PC++;
			switch (op)
			{
			case 0x11: //RL C - rotate left through carry
				temp = c;
				c <<= 1;
				c += (f & 0x10) >> 4;
				(temp & 0x80) != 0 ? f |= 0x10 : f &= 0xef;
				c == 0 ? f |= 0x10 : f &= 0x7f;
				f &= 0x9F;
				break;
			case 0x7C:	//BIT 7,H - test bit 7 in register H
				(h & 0x80) != 0 ? f &= 0x7F : f |= 0x80;
				f &= 0xBF;
				f |= 0x20;
				break;
			default:
				SetConsoleTextAttribute(hConsole, 14);
				std::cout << "0x" << std::hex << PC - 1 <<": Warning: Unimplemented opcode 0xCB" << std::hex << (int)op << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
				working = false;
			}
			m = 2;
			t = 8;
			break;
		default:		
			SetConsoleTextAttribute(hConsole, 14);
			std::cout << "0x" << std::hex << PC << ": Warning: Unimplemented opcode 0x" << std::hex << (int)op  << std::endl;
			SetConsoleTextAttribute(hConsole, 15);
			working = false;
		}
		PC++;
	}
}
