#include "draw_console.h"

#include <conio.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

static int cursor_x = 0, cursor_y = 0;

/* Key */
/**
 * https://stackoverflow.com/a/54581468
*/
int GetSpecialChar() {
    char ch = _getch();
    switch (ch) {
        case -32: // 224
            switch(ch = _getch()) { // the real value
                case kKeyUp:
                    // code for arrow up
                    return kKeyUp;
                case kKeyDown:
                    // code for arrow down
                    return kKeyDown;
                case kKeyRight:
                    // code for arrow right
                    return kKeyRight;
                case kKeyLeft:
                    // code for arrow left
                    return kKeyLeft;
                default:
                    break;
            }

            return 0;
        case '\r':
        case '\n':
            // Enter is a new line, which is CRLF or LF (Windows or Linux).
            return kKeyEnter;
        case '\b':
            return kKeyBackspace;
        case kKeyEsc:
            return kKeyEsc;
        default:
            if ('a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z') 
                return ch;

            return 0;
    }
}

void ReturnCursorPos(int &x, int &y) {
    x = cursor_x;
    y = cursor_y;
}

/* Cursor Control */
void GoToCursorPos(int row, int column) {
    cout << "\x1b[" << row << ';' << column << 'H';
    cursor_x = row;
    cursor_y = column;
}

/* Color */
string SetColor(int text_mode, int text, int background) {
    stringstream ss;
    ss << "\x1b[";
    if (text_mode != 0) {
        ss << text_mode << ';';
    }
    if (text != 0) {
        ss << text;
        if (background != 0) {
            ss << ';';
        }
    }
    if (background != 0) {
        ss << background;   
    }
    ss << "m";
    return ss.str();
}

string SetColor(int text, int background) {
    return SetColor(0, text, background);
}

string SetColor(int text) {
    return SetColor(text, 0);
}

void DrawBackgroundCell(const string& filename, int x, int y) {
    ifstream file(filename);
    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    // In ảnh từ vị trí (x, y)
    for (size_t i = 0; i < lines.size(); ++i) {
        // In background tại vị trí x, y
        GoToCursorPos(y + i, x);
        cout << lines[i] << endl; 
    }
    file.close();
}

void DrawBoardFrame(int x, int y, int h, int w) {
    cout << SetColor(kBold, kMagenta, 0);
    GoToCursorPos(y, x); cout << (char)201;
    for(int ix = x + 1; ix < x + w; ix++){
        GoToCursorPos(y, ix); cout << char(205);
        GoToCursorPos(y + h, ix); cout << char(205);
    }
    GoToCursorPos(y, x + w); cout << (char)187;
    GoToCursorPos(y + h, x); cout << (char)200;
    for(int iy = y + 1; iy < y + h ; iy++){
        GoToCursorPos(iy, x); cout << char(186);
        GoToCursorPos(iy, x + w); cout << char(186);
    }
    GoToCursorPos(y + h, x + w); cout << (char)188;
    cout << SetColor(0, kDefault, kBackgroundDefault);
}

void SetBackgroundColor(int color) {         
    // cout << "\x1b[0K";
    cout << "\x1b[" << color << "m";
}

/* Draw */
void EraseScreen() {
    cout << "\x1b[2J";
}
