#include "structs.h"

#include "linked_list.h"

#ifndef GAME_BOARD_H_
#define GAME_BOARD_H_

/**
 * (rows * columns) must be even. 
*/
Board& GenerateBoard(int rows, int columns);

void DeleteBoard(Board &board);

void ShuffleBoard(Board &board);

/**
 * In actual board, not border.
*/
bool IsInside(Board &board, int x, int y);

bool IsCellEmpty(Board &board, int x, int y);

void RemoveCell(Board &board, int x, int y);

void GoToBoardCell(Board &board, int x, int y);

void AddChosenCell(Board &board, int x, int y);

void RemoveChosenCell(Board &board, int x, int y);

/**
 * A matching is a correct path between 2 cells (I, U, L, Z)
*/
bool CheckMatching(Board &board);

//bool TraverseTo(Board &board, int x, int y, int dest_x, int dest_y, List &path, int turn_type = -1);
#endif // GAME_BOARD_H_