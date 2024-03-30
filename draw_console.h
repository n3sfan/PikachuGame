#ifndef GAME_DRAW_CONSOLE_H_
#define GAME_DRAW_CONSOLE_H_
#include "structs.h"

/* Constants */
const int kBlack = 30;
const int kRed = 31;
const int kGreen = 32;
const int kYellow = 33;
const int kBlue = 34;
const int kMagenta = 35;
const int kCyan = 36;
const int kWhite = 37;
// Reset previous color and text effect.
const int kDefault = 39;
// Reset ALL colors and text effects, use kDefault to reset instead.
const int kReset = 0;

/* Text mode */
const int kBold = 1;
const int kItalic = 2;
const int kUnderline = 3;
const int kBlinking = 4;
const int kStrikethrough = 9;

const int kBackgroundBlack = 40;
const int kBackgroundRed = 41;
const int kBackgroundGreen = 42;
const int kBackgroundYellow = 43;
const int kBackgroundBlue = 44;
const int kBackgroundMagenta = 45;
const int kBackgroundCyan = 46;
const int kBackgroundWhite = 47;
const int kBackgroundDefault = 49;

const int kCellWidth = 9;
const int kCellHeight = 5;

/* Keys of GetSpecialChar */
const int kKeyEsc = 27;
const int kKeyUp = 72;
const int kKeyDown = 80;
const int kKeyRight = 77;
const int kKeyLeft = 75;
const int kKeyEnter = '\n';
const int kKeyBackspace = '\b';

/* Key */
int GetSpecialChar();

/* Cursor Control */
void ReturnCursorPos(int &row, int &column);

void GoToCursorPos(int row, int column);

/* Color */
/**
 * text: constants kBlack, kRed,..., kWhite or kReset.
 * background: constants kBackgroundBlack, kBackgroundRed,... kBackgroundWhite or kReset.
 * text_mode: constants kBold, kItalic,... or kStrikethrough.
*/
string SetColor(int text_mode, int text, int background);

string SetColor(int text, int background);

string SetColor(int text);

void DrawBackgroundCell(const string& filename, int x, int y);
/* Draw */
void EraseScreen();

#endif // GAME_DRAW_CONSOLE_H_