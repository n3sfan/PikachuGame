#include <windows.h>

#include <iostream>

#include "draw_console.h"
#include "game_stage.h"

using namespace std;

HANDLE h_stdout;

/**
 * Switch between menu, game, leaderboard screens.
*/
void SwitchScreen(int screen_type) {

}

int main() {
    h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (h_stdout == INVALID_HANDLE_VALUE) {
        cout << "Cannot get Console Handle: " << GetLastError() << '\n';
        return 1;
    }

    DWORD consoleMode;
    GetConsoleMode(h_stdout, &consoleMode);
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(h_stdout, consoleMode)) {
        cout << "Cannot run Game, please upgrade to >= Windows 10 TH2.\n" << GetLastError() << "\n";
        return 1;
    }

    EraseScreen();
    GoToCursorPos(0, 0);
    // Make cursor invisible
    cout << "\x1b[?25l";

    // Game
    cout << "\x1b[31;42mHello";
    while (true) {

    }

    //Board &board = StartGame(4, 6);
    
    //while (true) {
        //int ch = GetSpecialChar();
        //OnKeyPressed(board, ch);
        ////cout << chdd << '\n';
    //}

    return 0;
}