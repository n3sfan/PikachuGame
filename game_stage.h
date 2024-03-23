#ifndef GAME_GAME_STAGE_H
#define GAME_GAME_STAGE_H

#include "structs.h"
#include "draw_console.h"

Board& StartGame(int rows, int columns);

void StopGame(Board &board);

void OnKeyPressed(Board &board, char key);

void MoveToCell(Board &board, int x, int y);

void ChooseCell(Board &board, int x, int y);

/**
 * Draw a cell which has top left corner at (x, y).
 * (x, y) is a position in console screen.
 * 
 * c: Character which cell displays.
*/
void DrawCell(int x, int y, char c, int char_mode = 0, int char_color = kWhite, int background_color = kBackgroundBlack);

/**
 * Draw a cell from background text file.
 * file_x, file_y: character position in text file
 * x, y: position in console screen.
*/
void DrawBackgroundCell(string filename, int file_x, int file_y, int x, int y);

/**
 * path: Path from 1st matched cell to 2nd one.
*/
void DrawMatching(const Cell *path, int n);

/**
 * Outer border surrounding (actual) board.
*/
void DrawBorder(Board &board);

void DrawBoard(Board &board);

void NotifyCell(Board &board, int x, int y, int state);

/**
 * cell's position in console screen => cell's position in board
*/
void CellToPos(const Cell &c, int &x, int &y);
#endif // GAME_GAME_STAGE_H 