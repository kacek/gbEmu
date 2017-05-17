#include "Memory.h"



Memory::Memory()
{
}


Memory::~Memory()
{
}

void Memory::init()
{
	ZeroMemory(&RAM, sizeof(RAM));
}
