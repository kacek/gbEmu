#pragma once
#include "MemoryBUS.h"
class z80CPU
{
public:
	z80CPU();
	~z80CPU();
	void init(MemoryBUS* memBUS);
	void execute();
	void reset();

private:
	MemoryBUS* BUS;
	unsigned char a, b, c, d, e, h, l, f;
	unsigned char m, t;
	unsigned short PC, SP, temp;
	bool working;
	unsigned char op;
	bool en_interr;
	char en_interr_timer;
};

