#include "stdafx.h"
#include "GameBoy.h" 

void main()
{
	char DMG_file[MAX_PATH], ROM_file[MAX_PATH];
	GameBoy* App = new GameBoy();
	App->init();
	MessageBox(NULL, "Choose DMG(GameBoy bios)", "Info", MB_OK | MB_ICONINFORMATION);
	OPENFILENAMEA file_chooser;
	ZeroMemory(&DMG_file, sizeof(DMG_file));
	ZeroMemory(&file_chooser, sizeof(file_chooser));
	ZeroMemory(&ROM_file, sizeof(ROM_file));
	file_chooser.lStructSize = sizeof(OPENFILENAMEA);
	file_chooser.hwndOwner = NULL;
	file_chooser.lpstrFile = DMG_file;
	file_chooser.nMaxFile = MAX_PATH;
	file_chooser.Flags = OFN_EXPLORER;
	file_chooser.lpstrFilter = NULL;
	file_chooser.lpstrCustomFilter = NULL;
	file_chooser.nFilterIndex = 0;
	file_chooser.lpstrFileTitle = NULL;
	file_chooser.lpstrInitialDir = NULL;
	file_chooser.lpstrTitle = NULL;
	if (GetOpenFileNameA(&file_chooser))
	{
		std::cout << "You chose the file \"" << DMG_file << "\"\n";
		App->loadBIOS(DMG_file);
	}
	else
	{
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}
	MessageBox(NULL, "Choose ROM(GameBoy gmae file)", "Info", MB_OK | MB_ICONINFORMATION);
	file_chooser.lpstrFile = ROM_file;
	if (GetOpenFileNameA(&file_chooser))
	{
		std::cout << "You chose the file \"" << DMG_file << "\"\n";
		App->loadROM(ROM_file);
	}
	else
	{
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}

	system("pause");
}