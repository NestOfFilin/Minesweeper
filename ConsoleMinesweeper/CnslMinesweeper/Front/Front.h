#pragma once

#include <windows.h>
#include "../Logic/MineField.h"

#include "../resource.h"

#define blackFront 0x7
#define whiteFront 0x70

class Front
{
private:
	HANDLE hStdIn;
	HANDLE hStdOut;

	DWORD oldConsoleMode;
	DWORD curConsoleMode;

	bool WriteConsoleOutPut(const CHAR_INFO *chiBuffer, COORD bufSize, COORD bufCoord);
	bool WriteChar(wchar_t ch, COORD bufCoord);
	bool WriteCharInfo(CHAR_INFO chi, COORD bufCoord);

	bool ReadConsoleOutPut(CHAR_INFO *chiBuffer, COORD bufSize, COORD bufCoord);
	bool ReadCharInfo(CHAR_INFO& chi, COORD bufCoord);

	PCHAR_INFO CreateCharInfoSet(const wchar_t* charSet, unsigned setSize);
	PCHAR_INFO CreateCharInfoSetByChar(const wchar_t wch, unsigned setSize);

	unsigned char MainMenu();
	void Game(unsigned char wigth, unsigned char heigth, unsigned mineCnt);

public:
	Front();
	~Front();
};

/*⟶
╚ ╦ ☠ ☹ ☺ ║ ╝ ╔ ╗ ═ ⛳ ➰ ⟟
☀ ✱ ✲ ✳ ✴ ✵ ✶ ✷ ✸ ✹ ✺ ✻ ✼ ✽
✶ 
*/

/*
         1---------2---------3---------4-
12345678901234567890123456789012345678901
╔═══════════════════════════════════════╗
║           Minesweeper                 ║
║                                       ║
║   New Game                            ║
║   ⮞ Easy                              ║
║     Medium                            ║
║     Hard                              ║
║                                       ║
║    Exit                               ║
║                                       ║
║   Version: console.pre-pre-alpha.1    ║
╚═══════════════════════════════════════╝
*/

/*╟ ╢ ╦ ╩ ─ ┬ | ╧ ① ② ③ ④ ⑤ ⑥ ⑦ ⑧  ⚐ ⚑
* ０ １ ２ ３ ４ ５ ６ ７ ８ ９ ◴ ◷
╔═══════════════════════════════════════╗
║   ⛳✖10          ☺        ✹✖10     ║
╟───────────────────────────────────────╢
║        ╔════════════════════╗         ║
║        ║✹                  ║         ║
║        ║  ①                 ║         ║
║        ║                    ║         ║
║        ║                    ║         ║
║        ║                    ║         ║
║        ║                    ║         ║
║        ║                    ║         ║
║        ║                    ║         ║
║        ║                    ║         ║
║        ╚════════════════════╝         ║
╟────────────┬──────────────────────────╢
║    MOVE    | F or ◷ ─ mark cell      ║
║ ⯅ ⯆ ⯇ ⯈  | E or ◴ ─ open cell      ║
║ W A S D    |  ESC ─ exit              ║
╚════════════╧══════════════════════════╝
*/