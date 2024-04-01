#include <windows.h>
#include <conio.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>

#include "draw_console.h"
#include "game_stage.h"
#include "structs.h"
#include "board.h"
#include "leaderboard.h"

using namespace std;
int x = 50; int y = 15;

enum ScreenState {
    MENU,
    GAME,
    LEADERBOARD,
    MENULEVEL,
    MENUSIZE
};
enum ScreenCurrent{
    SCREEN_OF_GAME,
    SCREEN_OF_LEADERBOARD
};

/// Global variable to store the current screen state
ScreenState currentScreen = MENU;
// Global variable to store which screen's menu is active
ScreenCurrent Screen = SCREEN_OF_GAME;

HANDLE h_stdout;
static Board *board;

// /**
//  * Switch between menu, game, leaderboard screens.
// */

// Function to change text color
void setTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, static_cast<int>(color));
}

void Menu();
void MenuLevel();


// Function to clear previous output by filling with spaces
void ClearPreviousOutput(int startX, int startY, int width, int height) {
    cout << SetColor(0, 0, kBackgroundBlack);
    for (int i = startY; i < startY + height; ++i) {
        GoToCursorPos(i + 1, startX + 1);
        for (int j = startX; j < startX + width; ++j) {
            cout << " "; // Overwrite previous data with spaces
        }
    }
}

void Brigth_Bar(int x, int y, int w, int h, int b_color, string nd){
    ClearPreviousOutput(x, y, w - 2, 1);
    cout << SetColor(0, 0, b_color);
    for(int iy = y + 1; iy <= y + h - 1; iy++){
        for(int ix = x + 1; ix <= y + h - 1; ix++){
            GoToCursorPos(iy, ix); cout << " ";
        }
    }
    cout << SetColor(0, kWhite, 0);
    GoToCursorPos(y + 1, x + 1);
    cout << nd;
}

void Box(int x, int y, int w, int h, int t_color, int b_color, string nd){
    cout << SetColor(0, 0, b_color);
    for(int iy = y + 1; iy <= y + h - 1; iy++){
        for(int ix = x + 1; ix <= y + h - 1; ix++){
            GoToCursorPos(iy, ix); cout << " ";
        }
    }
    
    cout << SetColor(0, kWhite, 0);
    GoToCursorPos(y + 1, x + 1);
    cout << nd;
    // Draw borders
    cout << SetColor(0, 0 , kBackgroundBlack);
    cout << SetColor(0, t_color, 0);
    if(h <= 1 || w <= 1) return;
    for(int ix = x; ix <= x + w; ix++){
        GoToCursorPos(y, ix);
        cout << char(196);
        GoToCursorPos(y + h, ix);
        cout << char(196);
    }
    for(int iy = y; iy <= y + h; iy++){
        GoToCursorPos(iy, x);
        cout << char(179);
        GoToCursorPos(iy, x + w);
        cout << char(179);
    }
    GoToCursorPos(y,x); cout << char(218);
    GoToCursorPos(y, x + w); cout << char(191);
    GoToCursorPos(y + h, x); cout << char(192);
    GoToCursorPos(y + h, x + w); cout << char(217);
}

// Function to print characters with delay
void PrintSlowly(const string& text, int delay) {
    for (int i = 0; i < text.size(); i += 10) {
        for (int j = i; j < ((text.size() < i + 10) ? text.size() : i + 10); ++j) {
            cout << text[j] << flush; // Print character by character without buffering
        }
        this_thread::sleep_for(chrono::milliseconds(delay)); // Wait for a certain amount of time
    }
}

void MenuSizeStandard() {
    GoToCursorPos(y - 12, x - 20);
    cout << SetColor(0, kGreen, 0);
    DrawBackgroundCell("background2.txt", x - 40, y + 5);
    cout << SetColor(0, kYellow, 0);
    DrawBackgroundCell("background6.txt", x + 20, y - 5);
    cout << SetColor(0, kRed, 0);
    DrawBackgroundCell("background7.txt", x - 20, y - 12);
    cout << "\x1b[?25l";// Xóa nháy chuột
    // setting
    int w = 20;
    int h = 2; 
    int t_color = kWhite;
    int b_color = kBackgroundBlack;
    int b_color_sang = kBackgroundRed;
    string nd = "3 x 4";
    string nd2 = "4 x 6";
    string nd3 = "7 x 10";
    string nd4 = "Back"; 
    int sl = 4;
    // Print menu 
    cout << SetColor(0, 0, kBackgroundWhite);
    Box( x, y, w, h, t_color, b_color, nd);
    Box( x, y + 2, w, h, t_color, b_color, nd2);
    GoToCursorPos(y + 2, x); cout << char(195);
    GoToCursorPos(y + 2, x + 20); cout << char(180);
    Box( x, y + 4, w, h, t_color, b_color, nd3);
    GoToCursorPos(y + 4, x); cout << char(195);
    GoToCursorPos(y + 4, x + 20); cout << char(180);
    Box( x, y + 6, w, h, t_color, b_color, nd4);
    GoToCursorPos(y + 6, x); cout << char(195);
    GoToCursorPos(y + 6, x + 20); cout << char(180);
    // Bright bar animation
    Brigth_Bar(x, y, w, h, b_color_sang, nd);
    int xp = x; int yp = y; // Bright bar coordinates
    bool kt = true;
    int xcu = xp; int ycu = yp;
    int index = 1; // Bright bar position
    int indexcu = 1; // Previous bright bar position
    while(true){
        // Print
        if( kt == true){
            // back space
            GoToCursorPos(ycu, xcu);
            // Reset previous bright bar with corresponding indexcu
            switch (indexcu){
                case 1:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd);
                    break;
                case 2:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd3);
                    break;
                case 4:
                    Brigth_Bar(xcu, ycu, w, h , b_color, nd4);
                    break;
            } 
            xcu = xp; ycu = yp;
            // ---------
            switch (index){
                case 1:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd);
                    break;
                case 2:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd3);
                    break;
                case 4:
                    Brigth_Bar(xp, yp, w, h , b_color_sang, nd4);
                    break;
            }
            kt = false;
        }
        // Control // movement
        if(_kbhit()){
            char c = _getch();
            if(c == '\r'){
                cout << SetColor(0, kDefault, kBackgroundDefault);
                switch (index){
                    case 1:
                        // Go to 3 x 4 Standard
                        if(Screen == SCREEN_OF_GAME){ 
                            currentScreen = GAME;
                            EraseScreen();
                            board = &StartGame(3, 4, false);
                            return;
                            break;
                        }else{
                            currentScreen = LEADERBOARD;
                            EraseScreen();
                            DrawStageLeaderboard("3x4_standard.txt");
                            return;
                            break;
                        }
                       
                    case 2:
                        // Go to 4 x 6 Standard
                        if(Screen == SCREEN_OF_GAME){
                            currentScreen = GAME;
                            EraseScreen();
                            board = &StartGame(4, 6, false);
                            return;
                            break;
                        }else{
                            currentScreen = LEADERBOARD;
                            EraseScreen();
                            DrawStageLeaderboard("4x6_standard.txt");
                            return;
                            break;
                        }
                    case 3:
                        // Go to 7 x 10 Standard
                        if(Screen == SCREEN_OF_GAME){
                            currentScreen = GAME;
                            EraseScreen();
                            board = &StartGame(7, 10, false);
                            return;
                            break;
                        }else{
                            currentScreen = LEADERBOARD;
                            EraseScreen();
                            DrawStageLeaderboard("7x10_standard.txt");
                            return;
                            break;
                        }
                    case 4:
                        // Back to screen MENULEVEL
                        currentScreen = MENULEVEL;
                        EraseScreen();
                        MenuLevel();
                        return;
                        break;
                }
            }else if(c == -32){
                kt = true;
                c = _getch();
                if(c == kKeyUp){ // Press the up arrow key
                    if(yp != y)
                        yp -= 2;
                        if(yp == y + 4){
                            index = 3;
                            indexcu = 4;
                        }else if(yp == y + 2){
                            index = 2;
                            indexcu = 3;
                        }else if(yp == y){
                            index = 1;
                            indexcu = 2;
                        }
                    else 
                    {
                        yp = y + h*(sl - 1);
                        index = 4;
                        indexcu = 1;
                    }
                }else if(c == kKeyDown ){ // Press the down arrow key
                    if(yp != y + h*(sl - 1))
                        yp += 2;
                        if(yp == y + 2){
                            index = 2;
                            indexcu = 1;
                        }else if(yp == y + 4){
                            index = 3;
                            indexcu = 2;
                        }else if(yp == y + 6){
                            index = 4;
                            indexcu = 3;
                        }
                            
                    else 
                    {
                        yp = y;
                        index = 1;
                        indexcu = 4;
                    }
                }
            }
        }    
    }
}

void MenuSizeCollapsing(){
    GoToCursorPos(y - 12, x - 20);
    cout << SetColor(0, kGreen, 0);
    DrawBackgroundCell("background2.txt",x - 40, y + 5 );
    cout << SetColor(0, kYellow, 0);
    DrawBackgroundCell("background6.txt", x + 20, y - 5);
    cout << SetColor(0, kRed, 0);
    DrawBackgroundCell("background7.txt", x - 20, y - 12);
    // setting
    int w = 20;
    int h = 2; 
    int t_color = kWhite;
    int b_color = kBackgroundBlack;
    int b_color_sang = kBackgroundRed;
    string nd = "3 x 4";
    string nd2 = "4 x 6";
    string nd3 = "7 x 10"; 
    string nd4 = "Back";
    int sl = 4;
    // Print menu  
    cout << SetColor(0, 0, kBackgroundWhite);
    Box( x, y, w, h, t_color, b_color, nd);
    Box( x, y + 2, w, h, t_color, b_color, nd2);
    GoToCursorPos(y + 2, x); cout << char(195);
    GoToCursorPos(y + 2, x + 20); cout << char(180);
    Box( x, y + 4, w, h, t_color, b_color, nd3);
    GoToCursorPos(y + 4, x); cout << char(195);
    GoToCursorPos(y + 4, x + 20); cout << char(180);
    Box( x, y + 6, w, h, t_color, b_color, nd4);
    GoToCursorPos(y + 6, x); cout << char(195);
    GoToCursorPos(y + 6, x + 20); cout << char(180);
    // Bright bar animation
    Brigth_Bar(x, y, w, h, b_color_sang, nd);
    int xp = x; int yp = y; // Bright bar coordinates
    bool kt = true;
    int xcu = xp; int ycu = yp;
    int index = 1; // Bright bar position 
    int indexcu = 1; // Previous bright bar position
    while(true){
        // Print
        if( kt == true){
            // back space
            GoToCursorPos(ycu, xcu);
            // Reset previous bright bar with corresponding indexcu
            switch (indexcu){
                case 1:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd);
                    break;
                case 2:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd3);
                    break;
                case 4:
                    Brigth_Bar(xcu, ycu, w, h , b_color, nd4);
                    break;
            } 
            xcu = xp; ycu = yp;
            // ---------
            switch (index){
                case 1:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd);
                    break;
                case 2:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd3);
                    break;
                case 4:
                    Brigth_Bar(xp, yp, w, h , b_color_sang, nd4);
                    break;
            }
            kt = false;
        }
        // Control // movement
        if(_kbhit()){
            char c = _getch();
            if(c == '\r'){
                cout << SetColor(0, kDefault, kBackgroundDefault);
                switch (index){
                    case 1:
                        // Go to 3 x 4 Standard
                        if(Screen == SCREEN_OF_GAME){
                            currentScreen = GAME;
                            EraseScreen();
                            board = &StartGame(3, 4, true);
                            return;
                            break;
                        }else{
                            currentScreen = LEADERBOARD;
                            EraseScreen();
                            DrawStageLeaderboard("3x4_collapsing.txt");
                            return;
                            break;
                        }
                       
                    case 2:
                        // Go to 4 x 6 Standard
                        if(Screen == SCREEN_OF_GAME){
                            currentScreen = GAME;
                            EraseScreen();
                            board = &StartGame(4, 6, true);
                            return;
                            break;
                        }else{
                            currentScreen = LEADERBOARD;
                            EraseScreen();
                            DrawStageLeaderboard("4x6_collapsing.txt");
                            return;
                            break;
                        }
                    case 3:
                        // Go to 7 x 10 Standard
                        if(Screen == SCREEN_OF_GAME){
                            currentScreen = GAME;
                            EraseScreen();
                            board = &StartGame(7, 10, true);
                            return;
                            break;
                        }else{
                            currentScreen = LEADERBOARD;
                            EraseScreen();
                            DrawStageLeaderboard("7x10_collapsing.txt");
                            return;
                            break;
                        }
                    case 4:
                        // Back to screen MENULEVEL
                        currentScreen = MENULEVEL;
                        EraseScreen();
                        MenuLevel();
                        return;
                        break;
                }
            }else if(c == -32){
                kt = true;
                c = _getch();
                if(c == kKeyUp){ // Press the up arrow key
                    if(yp != y)
                        yp -= 2;
                        if(yp == y + 4){
                            index = 3;
                            indexcu = 4;
                        }else if(yp == y + 2){
                            index = 2;
                            indexcu = 3;
                        }else if(yp == y){
                            index = 1;
                            indexcu = 2;
                        }
                    else 
                    {
                        yp = y + h*(sl - 1);
                        index = 4;
                        indexcu = 1;
                    }
                }else if(c == kKeyDown ){ // Press the down arrow key
                    if(yp != y + h*(sl - 1))
                        yp += 2;
                        if(yp == y + 2){
                            index = 2;
                            indexcu = 1;
                        }else if(yp == y + 4){
                            index = 3;
                            indexcu = 2;
                        }else if(yp == y + 6){
                            index = 4;
                            indexcu = 3;
                        }
                            
                    else 
                    {
                        yp = y;
                        index = 1;
                        indexcu = 4;
                    }
                }
            }
        }    
    }

}
void MenuLevel(){
    GoToCursorPos(y - 12, x - 20);
    cout << SetColor(0, kGreen, 0);
    DrawBackgroundCell("background2.txt",x - 40, y + 5 );
    cout << SetColor(0, kYellow, 0);
    DrawBackgroundCell("background6.txt", x + 20, y - 5);
    cout << SetColor(0, kRed, 0);
    DrawBackgroundCell("background7.txt", x - 20, y - 12);
    // setting
    int w = 20;
    int h = 2; 
    int t_color = kWhite;
    int b_color = kBackgroundBlack;
    int b_color_sang = kBackgroundRed;
    string nd = "Standard";
    string nd2 = "Collapsing";
    string nd3 = "Back"; 
    int sl = 3;
    // Print menu
    cout << SetColor(0, 0, kBackgroundWhite);
    Box( x, y, w, h, t_color, b_color, nd);
    Box( x, y + 2, w, h, t_color, b_color, nd2);
    GoToCursorPos(y + 2, x); cout << char(195);
    GoToCursorPos(y + 2, x + 20); cout << char(180);
    Box( x, y + 4, w, h, t_color, b_color, nd3);
    GoToCursorPos(y + 4, x); cout << char(195);
    GoToCursorPos(y + 4, x + 20); cout << char(180);
    // Bright bar animation
    Brigth_Bar(x, y, w, h, b_color_sang, nd);
    int xp = x; int yp = y; // Bright bar coordinates
    bool kt = true;
    int xcu = xp; int ycu = yp;
    int index = 1; // Bright bar position 
    int indexcu = 1; // Previous bright bar position
    while(true){
        // Print
        if( kt == true){
            // back space
            GoToCursorPos(ycu, xcu);
            // Reset previous bright bar with corresponding indexcu
            switch (indexcu){
                case 1:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd);
                    break;
                case 2:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd3);
                    break;
            } 
            xcu = xp; ycu = yp;
            // ---------
            switch (index){
                case 1:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd);
                    break;
                case 2:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd3);
                    break;
            }
            kt = false;
        }
        // Control // movement
        if(_kbhit()){
            char c = _getch();
            if(c == '\r'){
                cout << SetColor(0, kDefault, kBackgroundDefault);
                switch (index){
                    case 1:
                        currentScreen = MENUSIZE;
                        EraseScreen();
                        MenuSizeStandard();
                        return;
                        break;
                    case 2:
                        EraseScreen();
                        currentScreen = MENUSIZE;
                        MenuSizeCollapsing();
                        return;
                        break;
                    case 3:
                        EraseScreen();
                        currentScreen = MENU;
                        Menu();
                        return;
                        break;
                }
            }else if(c == -32){
                kt = true;
                c = _getch();
                if(c == kKeyUp){ // Press the up arrow key
                    if(yp != y)
                        yp -= 2;
                        if(yp == y + 2){
                            index = 2;
                            indexcu = 3;
                        }else if(yp == y){
                            index = 1;
                            indexcu = 2;
                        }      
                    else 
                    {
                        yp = y + h*(sl -1);
                        index = 3;
                        indexcu = 1;
                    }
                }else if(c == kKeyDown ){ // Press the down arrow key 
                    if(yp != y + h*(sl - 1))
                        yp += 2;
                        if(yp == y + 2){
                            index = 2;
                            indexcu = 1;
                        }else if(yp == y + 4){
                            index = 3;
                            indexcu = 2;
                        }
                            
                    else 
                    {
                        yp = y;
                        index = 1;
                        indexcu = 3;
                    }
                }
            }
        }    
    }

}

void Menu(){
    GoToCursorPos(y - 12, x - 20);
    cout << SetColor(0, kGreen, 0);
    DrawBackgroundCell("background2.txt", x - 40, y + 5);
    cout << SetColor(0, kYellow, 0);
    DrawBackgroundCell("background6.txt", x + 20, y - 5);
    cout << SetColor(0, kRed, 0);
    DrawBackgroundCell("background7.txt", x - 20, y - 12);
    // setting
    int w = 20;
    int h = 2; 
    int t_color = kWhite;
    int b_color = kBackgroundBlack;
    int b_color_sang = kBackgroundRed;
    string nd = "Play Game";
    string nd2 = "Leaderboard";
    string nd3 = "Quit";
    int sl = 3;
    // Print menu 
    cout << SetColor(0, 0, kBackgroundWhite);
    Box( x, y, w, h, t_color, b_color, nd);
    Box( x, y + 2, w, h, t_color, b_color, nd2);
    GoToCursorPos(y + 2, x); cout << char(195);
    GoToCursorPos(y + 2, x + 20); cout << char(180);
    Box( x, y + 4, w, h, t_color, b_color, nd3);
    GoToCursorPos(y + 4, x); cout << char(195);
    GoToCursorPos(y + 4, x + 20); cout << char(180);
    
    // Bright bar animation
    Brigth_Bar(x, y, w, h, b_color_sang, nd);
    int xp = x; int yp = y; // Bright bar coordinates
    bool kt = true;
    int xcu = xp; int ycu = yp;
    int index = 1; // Bright bar position 
    int indexcu = 1; // Previous bright bar position

    while(true){
        // Print
        if( kt == true){
            // back space
            GoToCursorPos(ycu, xcu);
            // Reset previous bright bar with corresponding indexcu
            switch (indexcu){
                case 1:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd);
                    break;
                case 2:
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xcu, ycu, w, h, b_color, nd3);
                    break;
            } 
            xcu = xp; ycu = yp;
            // ---------
            switch (index){
                case 1:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd);
                    break;
                case 2:
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd2);
                    break;
                case 3: 
                    Brigth_Bar(xp, yp, w, h, b_color_sang, nd3);
                    break;
            }
            kt = false;
        }
        // Control // movement
        if(_kbhit()){
            char c = _getch();
            if(c == '\r'){
                cout << SetColor(0, kDefault, kBackgroundDefault);
                switch (index){
                    case 1:
                        Screen = SCREEN_OF_GAME;
                        currentScreen = MENULEVEL;
                        EraseScreen();
                        MenuLevel();
                        return;
                        break;
                    case 2:
                        Screen = SCREEN_OF_LEADERBOARD;
                        currentScreen = MENULEVEL;
                        EraseScreen();
                        MenuLevel();
                        return;
                        break;
                    case 3:
                        exit(0);
                        return;
                        break;
                }
            }else if(c == -32){
                kt = true;
                c = _getch();
                if(c == kKeyUp){ // Press the up arrow key
                    if(yp != y)
                        yp -= 2;
                        if(yp == y + 2){
                            index = 2;
                            indexcu = 3;
                        }else if(yp == y){
                            index = 1;
                            indexcu = 2;
                        }      
                    else 
                    {
                        yp = y + h*(sl -1);
                        index = 3;
                        indexcu = 1;
                    }
                }else if(c == kKeyDown ){ // Press the down arrow key
                    if(yp != y + h*(sl - 1))
                        yp += 2;
                        if(yp == y + 2){
                            index = 2;
                            indexcu = 1;
                        }else if(yp == y + 4){
                            index = 3;
                            indexcu = 2;
                        }
                            
                    else 
                    {
                        yp = y;
                        index = 1;
                        indexcu = 3;
                    }
                }
            }
        }    
    }
}




// void SwitchScreen(int screen_type) {

// }

int main() {
    HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

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
    // Maximize window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
    // Set font size
    CONSOLE_FONT_INFOEX cfi = {sizeof(cfi)};
    // Populate cfi with the screen buffer's current font info
    GetCurrentConsoleFontEx(h_stdout, FALSE, &cfi);
    // Modify the font size in cfi
    cfi.dwFontSize.X = 10;
    cfi.dwFontSize.Y = 20;
    // Use cfi to set the screen buffer's new font
    SetCurrentConsoleFontEx(h_stdout, FALSE, &cfi);

    EraseScreen();
    GoToCursorPos(0, 0);
    // Make cursor invisible
    cout << "\x1b[?25l";
    string asciiArt = R"(
/\ \  __/\ \        /\_ \
\ \ \/\ \ \ \     __\//\ \     ___    ___     ___ ___      __
 \ \ \ \ \ \ \  /'__`\\ \ \   /'___\ / __`\ /' __` __`\  /'__`\
  \ \ \_/ \_\ \/\  __/ \_\ \_/\ \__//\ \L\ \/\ \/\ \/\ \/\  __/
   \ `\___x___/\ \____\/\____\ \____\ \____/\ \_\ \_\ \_\ \____\
    '\/__//__/  \/____/\/____/\/____/\/___/  \/_/\/_/\/_/\/____/

            /\ \__
            \ \ ,_\   ___        ___   __  __  _ __
             \ \ \/  / __`\     / __`\/\ \/\ \/\`'__\
              \ \ \_/\ \L\ \   /\ \L\ \ \ \_\ \ \ \/
               \ \__\ \____/   \ \____/\ \____/\ \_\
                \/__/\/___/     \/___/  \/___/  \/_/

                 __      __      ___ ___      __
               /'_ `\  /'__`\  /' __` __`\  /'__`\
              /\ \L\ \/\ \L\.\_/\ \/\ \/\ \/\  __/
              \ \____ \ \__/.\_\ \_\ \_\ \_\ \____\
               \/___L\ \/__/\/_/\/_/\/_/\/_/\/____/
                 /\____/
                 \_/__/
)";
    // In ASCII art một cách từ từ với delay là 5 milliseconds
    PrintSlowly(asciiArt, 20);
    Sleep(800);

    while(true) {
        if (currentScreen == GAME) {
            OnGameUpdate(*board);
            OnKeyPressed(*board, GetSpecialChar());
        } else {
            EraseScreen();
            MenuSound();
            Menu();
            if (currentScreen == GAME)
                continue;

            char input = _getch();
            if(input == kKeyEsc){
                if (currentScreen == LEADERBOARD){
                    EraseScreen();
                    // menu();
                    currentScreen = MENU;
                } else if(currentScreen == GAME){
                    EraseScreen();
                    // menu();
                    currentScreen = MENU;
                }
            }
        }
    }
   
    // EraseScreen();
    // GoToCursorPos(0, 0);
    // // Make cursor invisible
    // cout << "\x1b[?25l";


    // // Game
    // // cout << "\x1b[31;42mHello";
    // // while (true) {

    // // }

    // Board &board = StartGame(4, 6);
    
    // while (true) {
    //     int ch = GetSpecialChar();
    //     OnKeyPressed(board, ch);
    //     //cout << chdd << '\n';
    // }

    return 0;
}