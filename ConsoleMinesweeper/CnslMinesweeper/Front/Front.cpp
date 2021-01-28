#include "Front.h"

bool Front::WriteConsoleOutPut(CHAR_INFO *chiBuffer, COORD bufSize, COORD bufCoord)
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

bool Front::WriteChar(wchar_t ch, COORD bufCoord)
{
    COORD buffSize = { 1u, 1u };
    CHAR_INFO cellInf[1] = { { { ch }, 7 } };
    return WriteConsoleOutPut(cellInf, buffSize, bufCoord);
}

Front::Front() 
{
    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    // if (hStdIn == INVALID_HANDLE_VALUE) надо чото сделать если ошибка
    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    // if (hStdOut == INVALID_HANDLE_VALUE) надо чото сделать если ошибка

    GetConsoleMode(hStdIn, &oldConsoleMode); // проверку написать
    curConsoleMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(hStdIn, curConsoleMode); // проверку написать

    Game();

    SetConsoleMode(hStdIn, oldConsoleMode); // проверку написать
}
Front::~Front() {}

void Front::MainMenu() {}
void Front::Game() 
{
    bool flag = true;

    unsigned char wigth = 10u,
        heigth = 10u;
    unsigned mineCnt = 15u;
    // create field
    MineField mineField = MineField(wigth, heigth, mineCnt);

    COORD crdBufCoord = { 0u, 0u };
    unsigned cellCount = wigth * heigth;
    PCHAR_INFO chiBuffer = new CHAR_INFO[cellCount];
    // render field
    for (unsigned i = 0; i < cellCount; i++)
    {
        chiBuffer[i] = { { L'\u258c' }, 7 };
    }
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
            if (mineField.GetStatus() == MineFieldState::Cleaned
                || mineField.GetStatus() == MineFieldState::Undermined)
            {
                fLoop = false;
                FlushConsoleInputBuffer(hStdIn);
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
                                        flag = WriteChar(static_cast<wchar_t>(L'\u2735'), cellCrd);
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
}