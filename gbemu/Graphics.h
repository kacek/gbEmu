#pragma once
class Graphics
{
public:
	Graphics();
	~Graphics();
	void write8bTile(unsigned short address, unsigned char value);
	void write16bTile(unsigned short address, unsigned short value);
	unsigned char read8bTile(unsigned short address);
	unsigned short read16bTile(unsigned short address);

	void write8bBG1(unsigned short address, unsigned char value);
	void write16bBG1(unsigned short address, unsigned short value);
	unsigned char read8bBG1(unsigned short address);
	unsigned short read16bBG1(unsigned short address);

	void write8bBG2(unsigned short address, unsigned char value);
	void write16bBG2(unsigned short address, unsigned short value);
	unsigned char read8bBG2(unsigned short address);
	unsigned short read16bBG2(unsigned short address);
private:
	unsigned char TileRAM[0x1800];
	unsigned char BGmap1[0x400];
	unsigned char BGmap2[0x400];
};

