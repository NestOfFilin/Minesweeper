#include "Front.h"

bool Front::WriteConsoleOutPut(const CHAR_INFO *chiBuffer, COORD bufSize, COORD bufCoord)
{
    SMALL_RECT writeRect;
    writeRect.Left = bufCoord.X;
    writeRect.Top = bufCoord.Y;
    writeRect.Right = bufCoord.X + bufSize.X - 1;
    writeRect.Bottom = bufCoord.Y + bufSize.Y - 1;
    return WriteConsoleOutputW(
        hStdOut, // screen buffer to write to 
        chiBuffer,        // buffer to copy from 
        bufSize,     // col-row size of chiBuffer 
        { 0u, 0u },    // top left src cell in chiBuffer 
        &writeRect);  // dest. screen buffer rectangle 
}

bool Front::ReadConsoleOutPut(CHAR_INFO *chiBuffer, COORD bufSize, COORD bufCoord)
{
    SMALL_RECT writeRect;
    writeRect.Left = bufCoord.X;
    writeRect.Top = bufCoord.Y;
    writeRect.Right = bufCoord.X + bufSize.X - 1;
    writeRect.Bottom = bufCoord.Y + bufSize.Y - 1;
    return ReadConsoleOutputW(
        hStdOut, // screen buffer to write to 
        chiBuffer,        // buffer to copy from 
        bufSize,     // col-row size of chiBuffer 
        { 0u, 0u },    // top left src cell in chiBuffer 
        &writeRect);  // dest. screen buffer rectangle 
}

bool Front::WriteChar(wchar_t ch, COORD bufCoord)
{
    COORD buffSize = { 1u, 1u };
    CHAR_INFO cellInf[1] = { { { ch }, 7 } };
    return WriteConsoleOutPut(cellInf, buffSize, bufCoord);
}

bool Front::WriteCharInfo(CHAR_INFO chi, COORD bufCoord)
{
    COORD buffSize = { 1u, 1u };
    return WriteConsoleOutPut(&chi, buffSize, bufCoord);
}

bool Front::ReadCharInfo(CHAR_INFO &chi, COORD bufCoord)
{
    COORD buffSize = { 1u, 1u };
    return ReadConsoleOutPut(&chi, buffSize, bufCoord);
}

PCHAR_INFO Front::CreateCharInfoSet(const wchar_t* charSet, unsigned setSize)
{
    const PCHAR_INFO chiBuffer = new CHAR_INFO[setSize];
    for (unsigned i = 0; i < setSize; i++)
    {
        chiBuffer[i] = { { charSet[i] }, blackFront };
    }
    return chiBuffer;
}

PCHAR_INFO Front::CreateCharInfoSetByChar(const wchar_t wch, unsigned setSize)
{
    const PCHAR_INFO chiBuffer = new CHAR_INFO[setSize];
    for (unsigned i = 0; i < setSize; i++)
    {
        chiBuffer[i] = { { wch }, blackFront };
    }
    return chiBuffer;
}

Front::Front() 
{
    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    // if (hStdIn == INVALID_HANDLE_VALUE) надо чото сделать если ошибка
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // if (hStdOut == INVALID_HANDLE_VALUE) надо чото сделать если ошибка

    GetConsoleMode(hStdIn, &oldConsoleMode); // проверку написать
    curConsoleMode = oldConsoleMode;
    curConsoleMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hStdIn, curConsoleMode); // проверку написать

    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(hStdOut, &cursor_info);
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(hStdOut, &cursor_info);

    bool fLoop = true;
    while (fLoop)
    {
        unsigned char mi = MainMenu();
        switch (mi)
        {
        case 0:
            Game(9, 9, 10);
            break;
        case 1:
            Game(16, 16, 40);
            break;
        case 2:
            Game(30, 16, 99);
            break;
        default:
            fLoop = false;
            break;
        }
    }

    SetConsoleMode(hStdIn, oldConsoleMode); // проверку написать
}
Front::~Front() {}

unsigned char Front::MainMenu()
{
    unsigned char wigth = 41u,
        heigth = 12u;
    const wchar_t *mmenuCharSet = L"╔═══════════════════════════════════════╗║              Minesweeper              ║║                                       ║║   New Game                            ║║    Easy                               ║║    Medium                             ║║    Hard                               ║║                                       ║║    Exit                               ║║                                       ║║ Version : console.pre - pre - alpha.1 ║╚═══════════════════════════════════════╝⮞ Easy⮞ Medium⮞ Hard⮞ Exit";

    unsigned cellCount = wigth * heigth;
    const PCHAR_INFO chiBuffer = CreateCharInfoSet(mmenuCharSet, cellCount + 26);
    const PCHAR_INFO emptyChiBuffer = CreateCharInfoSetByChar(L' ', cellCount);
    COORD crdBufCoord = { 0u, 0u };
    bool flag = true;
    flag = WriteConsoleOutPut(chiBuffer, { wigth, heigth }, crdBufCoord);

    unsigned const INPUT_RECORD_SIZE = 1u;
    DWORD cNumRead;
    INPUT_RECORD irInBuf[INPUT_RECORD_SIZE];
    COORD cursorCrd = { 0, 0 };
    char selectedMI = -1;
    CHAR_INFO chiBuf;
    bool fLoop = true, fChangeMI[4] = { false, false, false, false };
    while (fLoop)
    {
        ReadConsoleInput(
            hStdIn,      // input buffer handle 
            irInBuf,     // buffer to read into 
            INPUT_RECORD_SIZE,         // size of read buffer 
            &cNumRead); // number of records read 
        switch (irInBuf[0].EventType)
        {
        case KEY_EVENT: 
        {
            if (!irInBuf[0].Event.KeyEvent.bKeyDown || !irInBuf[0].Event.KeyEvent.wRepeatCount > 1)
                break;
            fLoop = false;
            selectedMI = 3;
            break;
        }
        case MOUSE_EVENT:
        {
            MOUSE_EVENT_RECORD mer = irInBuf[0].Event.MouseEvent;
            switch (mer.dwEventFlags)
            {
            case 0:
                if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                {
                    if (selectedMI == -1) break;
                    fLoop = false;
                }
                break;
            case MOUSE_MOVED:
            {
                cursorCrd = mer.dwMousePosition;
                switch (cursorCrd.Y)
                {
                case 4:
                    if (selectedMI == 0) break;
                    flag = WriteConsoleOutPut(chiBuffer + cellCount, { 6, 1 }, { 4, 4 });
                    selectedMI = 0;
                    fChangeMI[selectedMI] = true;
                    break;
                case 5:
                    if (selectedMI == 1) break;
                    flag = WriteConsoleOutPut(chiBuffer + cellCount + 6, { 8, 1 }, { 4, 5 });
                    selectedMI = 1;
                    fChangeMI[selectedMI] = true;
                    break;
                case 6:
                    if (selectedMI == 2) break;
                    flag = WriteConsoleOutPut(chiBuffer + cellCount + 14, { 6, 1 }, { 4, 6 });
                    selectedMI = 2;
                    fChangeMI[selectedMI] = true;
                    break;
                case 8:
                    if (selectedMI == 3) break;
                    flag = WriteConsoleOutPut(chiBuffer + cellCount + 20, { 6, 1 }, { 4, 8 });
                    selectedMI = 3;
                    fChangeMI[selectedMI] = true;
                    break;
                default:
                    if (selectedMI == -1) break;
                    selectedMI = -1;
                    break;
                }
                for (int i = 0; i < 4; i++)
                {
                    if (fChangeMI[i] && selectedMI != i)
                    {
                        SHORT y = (i == 3 ? 8 : i + 4);
                        unsigned offset = wigth * y + 3;
                        flag = WriteConsoleOutPut(
                            chiBuffer + offset,
                            { 10, 1 },
                            { 3, y });
                        fChangeMI[i] = false;
                    }
                }
                break;
            }
            }
        }
        }
    }
    flag = WriteConsoleOutPut(emptyChiBuffer, { wigth, heigth }, crdBufCoord);
    delete[] emptyChiBuffer;
    delete[] chiBuffer;
    FlushConsoleInputBuffer(hStdIn);
    return selectedMI;
}

void Front::Game(unsigned char wigth, unsigned char heigth, unsigned mineCnt)
{
    bool flag = true;

    // create field
    MineField mineField = MineField(wigth, heigth, mineCnt);

    COORD crdBufCoord = { 0u, 0u };
    unsigned cellCount = wigth * heigth;
    const PCHAR_INFO chiBuffer = CreateCharInfoSetByChar(L'\u258c', cellCount);
    const PCHAR_INFO emptyChiBuffer = CreateCharInfoSetByChar(L' ', cellCount);
    // render field
    flag = WriteConsoleOutPut(chiBuffer, { wigth, heigth }, crdBufCoord);

    // actions
    unsigned const INPUT_RECORD_SIZE = 1u;
    DWORD cNumRead;
    INPUT_RECORD irInBuf[INPUT_RECORD_SIZE];
    CONSOLE_SCREEN_BUFFER_INFO consoleScreenBufferInfo;
    COORD cursorCrd;
    MineCellState cellState;
    bool fLoop = true;
    
    while (fLoop)
    {
        // Wait for the events.
        if (!ReadConsoleInput(
            hStdIn,      // input buffer handle 
            irInBuf,     // buffer to read into 
            INPUT_RECORD_SIZE,         // size of read buffer 
            &cNumRead)) // number of records read 
            break;

        // Dispatch the events to the appropriate handler.
        switch (irInBuf[0].EventType)
        {
        case KEY_EVENT: // keyboard input 
            if (!irInBuf[0].Event.KeyEvent.bKeyDown || !irInBuf[0].Event.KeyEvent.wRepeatCount > 1)
                break;
            if (mineField.GetStatus() == MineFieldState::Cleaned
                || mineField.GetStatus() == MineFieldState::Undermined)
            {
                fLoop = false;
            }
            break;
        case MOUSE_EVENT: // mouse input 
            if (mineField.GetStatus() == MineFieldState::Razminirovivaetsya)
            {
                MOUSE_EVENT_RECORD mer = irInBuf[0].Event.MouseEvent;
                switch (mer.dwEventFlags)
                {
                case 0:
                    GetConsoleScreenBufferInfo(hStdOut, &consoleScreenBufferInfo);
                    cursorCrd = consoleScreenBufferInfo.dwCursorPosition;

                    if ((cursorCrd.X >= 0 && cursorCrd.X <= wigth - 1) 
                        && (cursorCrd.Y >= 0 && cursorCrd.Y <= heigth - 1))
                    {
                        MineCell cell = mineField(cursorCrd.X, cursorCrd.Y);
                        cellState = cell.GetStatus();
                        if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
                        {
                            switch (cellState)
                            {
                            case MineCellState::CloseCell:
                            {
                                OpenedMineCells* openedCells = mineField.OpenCell(cursorCrd.X, cursorCrd.Y);
                                while (openedCells != nullptr)
                                {
                                    COORD cellCrd = { openedCells->x, openedCells->y };
                                    unsigned char cellVal = openedCells->CountOfMineAround;
                                    if (cellVal != 9)
                                    {
                                        if (cellVal == 0)
                                        {
                                            flag = WriteChar(L' ', cellCrd);
                                        }
                                        else
                                            flag = WriteChar((L'0' + cellVal), cellCrd);
                                    }
                                    else
                                    {
                                        flag = WriteChar((L'\u2735'), cellCrd);
                                    }
                                    OpenedMineCells* q = openedCells;
                                    openedCells = openedCells->Next;
                                    delete q;
                                }
                                break;
                            }
                            default:
                                break;
                            }
                        }
                        else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
                        {
                            switch (cellState)
                            {
                            case MineCellState::CloseCell:
                            case MineCellState::MarkedCell:
                                cellState = mineField.MarkCell(cursorCrd.X, cursorCrd.Y);
                                if (cellState == MineCellState::MarkedCell)
                                    flag = WriteChar(L'\u2690', cursorCrd);
                                else
                                    flag = WriteChar(L'\u258c', cursorCrd);
                                break;
                            default:
                                break;
                            }
                        }
                        else
                        {
                        }
                    }
                    break;
                case DOUBLE_CLICK:
                    break;
                case MOUSE_MOVED:
                    SetConsoleCursorPosition(hStdOut, mer.dwMousePosition);
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
    flag = WriteConsoleOutPut(emptyChiBuffer, { wigth, heigth }, crdBufCoord);
    delete[] emptyChiBuffer;
    delete[] chiBuffer;
    FlushConsoleInputBuffer(hStdIn);
}