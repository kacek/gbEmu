#pragma once
#include "MemoryBUS.h"
class z80CPU
{
public:
	z80CPU();
	~z80CPU();
	void init(MemoryBUS* memBUS);

private:
	MemoryBUS* BUS;

};

