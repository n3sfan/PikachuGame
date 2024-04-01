#ifndef GAME_GAME_STAGE_H
#define GAME_GAME_STAGE_H

#include <chrono>

#include "structs.h"
#include "draw_console.h"

namespace Game {
    extern chrono::_V2::system_clock::time_point begin_time, begin_music_time;
    extern int m, n;
    extern string background;
}


Board& StartGame(int rows, int columns, bool linked_list);

void StopGame(Board &board);

bool IsGameFinished(Board &board);

void OnGameUpdate(Board &board);

void OnKeyPressed(Board &board, char key);

void MoveToCell(Board &board, int x, int y);

void ChooseCell(Board &board, int x, int y);

void GameRemoveCell(Board &board, Cell cell, bool redraw_rows);

void MenuSound();

void GameSound();
/**
 * Is called when Player creates a matching, remove the cells.
*/
void GameRemoveCell(Board &board, Cell cell);

/**
 * Draw a cell which has top left corner at (x, y).
 * (x, y) is a position in console screen.
 * 
 * c: Character which cell displays.
*/
void DrawCell(int x, int y, char c, int char_mode = 0, int char_color = kDefault, int background_color = kBackgroundDefault);

void DrawEmptyCell(Cell c, int char_mode = 0, int char_color = kDefault, int background_color = kBackgroundDefault, bool clear = true);

/**
 * Draw a cell from background text file.
 * file_x, file_y: character position in text file
 * x, y: position in console screen.
*/
// void DrawBackgroundCell(string filename, int file_x, int file_y, int x, int y);
/**
 * path: Path from 1st matched cell to 2nd one.
*/
void DrawMatching(const Cell *path, int n, bool clear);

/**
 * Outer border surrounding (actual) board.
*/
void DrawBorder(Board &board);

/**
 * Draw boards starting from row x.
*/
void DrawBoard(Board &board, int x);

void DrawBoard(Board &board);

void DrawBackgroundCell(string filename, int file_x, int file_y, int x, int y, int h, int w);

/*
* Draw a row from cell (x, y) at row's cursor pos
*/
void DrawRow(Board &board, int x, int y);

void NotifyCell(Board &board, int x, int y, int state);

/**
 * cell's position in board = > cell's position in console screen
*/
void CellToPos(const Cell &c, int &x, int &y);

/* Ending Score */
void DrawEndingScoreScreen();
#endif // GAME_GAME_STAGE_H 