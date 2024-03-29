#include <windows.h>

#include <iostream>

#include "draw_console.h"
#include "game_stage.h"
#include <conio.h>
#include <vector>
#include "leaderboard.h"

using namespace std;
int x = 50; int y = 15;

enum ScreenState {
    MENU,
    GAME,
    LEADERBOARD
};

// Biến global lưu trạng thái màn hình hiện tại
ScreenState currentScreen = MENU;



// HANDLE h_stdout;

// /**
//  * Switch between menu, game, leaderboard screens.
// */
// enum class MenuOption { PLAY_GAME, LEADERBOARD, QUIT };

// // Hàm thay đổi màu chữ
// void setTextColor(int color) {
//     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
//     SetConsoleTextAttribute(hConsole, static_cast<int>(color));
// }


// Xóa nháy chuột 
void Nocursortype()
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
// Xóa dữ liệu bằng khoảng trắng
void clearPreviousOutput(int startX, int startY, int width, int height) {
    cout << SetColor(0, 0, kBackgroundBlack);
    for (int i = startY; i < startY + height; ++i) {
        GoToCursorPos(i + 1, startX + 1);
        for (int j = startX; j < startX + width; ++j) {
            cout << " "; // Ghi đè dữ liệu trước đó bằng khoảng trắng
        }
    }
}
void thanh_sang(int x, int y, int w, int h, int b_color, string nd){
    clearPreviousOutput(x, y, w - 2, 1);
    cout << SetColor(0, 0, b_color);
    for(int iy = y + 1; iy <= y + h - 1; iy++){
        for(int ix = x + 1; ix <= y + h - 1; ix++){
            GoToCursorPos(iy, ix); cout << " ";
        }
    }
    cout << SetColor(0, kBlack, 0);
    GoToCursorPos(y + 1, x + 1);
    cout << nd;

}

void box(int x, int y, int w, int h, int t_color, int b_color, string nd){
    cout << SetColor(0, 0, b_color);
    for(int iy = y + 1; iy <= y + h - 1; iy++){
        for(int ix = x + 1; ix <= y + h - 1; ix++){
            GoToCursorPos(iy, ix); cout << " ";
        }
    }
    
    cout << SetColor(0, kBlack, 0);
    GoToCursorPos(y + 1, x + 1);
    cout << nd;
    // Ve vien
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


void menu(){
    
    GoToCursorPos(y - 12, x - 20);
    cout << SetColor(0, kRed, 0);
    DrawBackgroundCell("background.txt", x - 20, y - 12);
    Nocursortype();
    // setting
    int w = 20;
    int h = 2; 
    int t_color = kGreen;
    int b_color = kBackgroundBlue;
    int b_color_sang = kBackgroundRed;
    string nd = "Play Game";
    string nd2 = "Leaderboard";
    string nd3 = "Quit";
    int sl = 3;
    // In ra menu 
    box( x, y, w, h, t_color, b_color, nd);
    box( x, y + 2, w, h, t_color, b_color, nd2);
    GoToCursorPos(y + 2, x); cout << char(195);
    GoToCursorPos(y + 2, x + 20); cout << char(180);
    box( x, y + 4, w, h, t_color, b_color, nd3);
    GoToCursorPos(y + 4, x); cout << char(195);
    GoToCursorPos(y + 4, x + 20); cout << char(180);
    
    // Chuyển động thanh sáng
    thanh_sang(x, y, w, h, b_color_sang, nd);
    int xp = x; int yp = y; // Tọa độ thanh sáng
    bool kt = true;
    int xcu = xp; int ycu = yp;
    int index = 1; // Vị trí của thanh sáng 
    int indexcu = 1; // Vị trí cũ của thanh sáng
    int test = 1;

    while(true){
        // in
        if( kt == true){
            // back space
            GoToCursorPos(ycu, xcu);
            // reset thanh sáng cũ với indexcu tương ứng
            switch (indexcu){
                case 1:
                    thanh_sang(xcu, ycu, w, h, b_color, nd);
                    break;
                case 2:
                    thanh_sang(xcu, ycu, w, h, b_color, nd2);
                    break;
                case 3: 
                    thanh_sang(xcu, ycu, w, h, b_color, nd3);
                    break;
            } 
            xcu = xp; ycu = yp;
            // ---------
            switch (index){
                case 1:
                    thanh_sang(xp, yp, w, h, b_color_sang, nd);
                    break;
                case 2:
                    thanh_sang(xp, yp, w, h, b_color_sang, nd2);
                    break;
                case 3: 
                    thanh_sang(xp, yp, w, h, b_color_sang, nd3);
                    break;
            }
            kt = false;
        }
        // điều khiển // di chuyển
        if(_kbhit()){
            
            char c = _getch();
            if(c == '\r'){
                cout << SetColor(0, kDefault, kBackgroundDefault);
                switch (index){
                    case 1:
                        // Vào Game
                        currentScreen = GAME;
                        return;
                        break;
                    case 2:
                        EraseScreen();
                        DrawStageLeaderboard();
                        currentScreen = LEADERBOARD;
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
                if(c == kKeyUp){ // Nhấn phím mũi tên đi lên
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
                }else if(c == kKeyDown ){ // Nhấn phím mũi tên đi xuống 
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
    // vector<Player> leaderboard = readLeaderboard("stage_filename.txt");
    // DrawStageLeaderboard(leaderboard);
    // currentScreen = LEADERBOARD;
    // _getch();
    // DrawBackgroundCell("background1.txt", 10 , 10);
    // _getch();
    while(true){
        EraseScreen();
        menu();
        char input = _getch();
        if(input == kKeyEsc){
            if(currentScreen == LEADERBOARD){
                EraseScreen();
                // menu();
                currentScreen = MENU;
            }else if(currentScreen == GAME){
                EraseScreen();
                // menu();
                currentScreen = MENU;
            }
        }
    }


    

    // h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    // if (h_stdout == INVALID_HANDLE_VALUE) {
    //     cout << "Cannot get Console Handle: " << GetLastError() << '\n';
    //     return 1;
    // }

    // DWORD consoleMode;
    // GetConsoleMode(h_stdout, &consoleMode);
    // consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    // if (!SetConsoleMode(h_stdout, consoleMode)) {
    //     cout << "Cannot run Game, please upgrade to >= Windows 10 TH2.\n" << GetLastError() << "\n";
    //     return 1;
    // }

    // EraseScreen();
    // GoToCursorPos(0, 0);
    // // Make cursor invisible
    // cout << "\x1b[?25l";

    // // Menu
    // int selectedIndex = 0;
    // MenuOption selectedOption;

    // while (true) {
    //     selectedOption = drawMenu(selectedIndex);
    //     switch (selectedOption) {
    //         case MenuOption::PLAY_GAME:
    //             //  Bắt đầu game           
    //             break;
    //         case MenuOption::LEADERBOARD:
    //             //  Chuyển đến leaderboard          
    //             break;
    //         case MenuOption::QUIT:
    //             return 0;
    //     }
    // }

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