#pragma once

#include <windows.h>
#include "../Logic/MineField.h"

#include "../resource.h"

class Front
{
private:
	HANDLE hStdIn;
	HANDLE hStdOut;

	DWORD oldConsoleMode;
	DWORD curConsoleMode;

	bool WriteConsoleOutPut(CHAR_INFO* chiBuffer, COORD bufSize, COORD bufCoord);
	bool WriteChar(wchar_t ch, COORD bufCoord);

	void MainMenu();
	void Game();

public:
	Front();
	~Front();
};

/*
╚ ╦ ☠ ☹ ☺ ║ ╝ ╔ ╗ ═ ⛳ ➰ ⟟
☀ ✱ ✲ ✳ ✴ ✵ ✶ ✷ ✸ ✹ ✺ ✻ ✼ ✽
✶ 
*/

/*
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