#include "Graphics.h"



Graphics::Graphics()
{
}


Graphics::~Graphics()
{
}

void Graphics::write8bTile(unsigned short address, unsigned char value)
{
	TileRAM[address] = value;
}

void Graphics::write16bTile(unsigned short address, unsigned short value)
{
	TileRAM[address] = (value & 0xFF00) >> 8;
	TileRAM[address] = (value & 0x00FF);
}

unsigned char Graphics::read8bTile(unsigned short address)
{
	return TileRAM[address];
}

unsigned short Graphics::read16bTile(unsigned short address)
{
	return (TileRAM[address]<<8)+TileRAM[address+1];
}

void Graphics::write8bBG1(unsigned short address, unsigned char value)
{
	BGmap1[address] = value;
}

void Graphics::write16bBG1(unsigned short address, unsigned short value)
{
	BGmap1[address] = (value & 0xFF00) >> 8;
	BGmap1[address] = (value & 0x00FF);
}

unsigned char Graphics::read8bBG1(unsigned short address)
{
	return BGmap1[address];
}

unsigned short Graphics::read16bBG1(unsigned short address)
{
	return (BGmap1[address] << 8) + BGmap1[address + 1];
}

void Graphics::write8bBG2(unsigned short address, unsigned char value)
{
	BGmap2[address] = value;
}

void Graphics::write16bBG2(unsigned short address, unsigned short value)
{
	BGmap2[address] = (value & 0xFF00) >> 8;
	BGmap2[address] = (value & 0x00FF);
}

unsigned char Graphics::read8bBG2(unsigned short address)
{
	return BGmap2[address];
}

unsigned short Graphics::read16bBG2(unsigned short address)
{
	return (BGmap2[address] << 8) + BGmap2[address + 1];
}