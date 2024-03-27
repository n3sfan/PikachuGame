#include "structs.h"

#include "linked_list.h"

#ifndef GAME_BOARD_H_
#define GAME_BOARD_H_

/**
 * (rows * columns) must be even. 
*/
Board& GenerateBoard(int rows, int columns);

void DeleteBoard(Board &board);

/**
 * Shuffle to create available matchings.
*/
void ShuffleBoard(Board &board);

/**
 * Naive algorithm. Check O(m^2 * n^2) cells.
*/
bool SuggestNextMove(Board &board, Cell &c1, Cell &c2);

/**
 * In actual board, not border.
*/
bool IsInside(Board &board, int x, int y);

bool IsCellEmpty(Board &board, int x, int y);

void BoardRemoveCell(Board &board, int x, int y);

void BoardGoToCell(Board &board, int x, int y);

void AddChosenCell(Board &board, int x, int y);

void RemoveChosenCell(Board &board, int x, int y);

/**
 * A matching is a correct path between 2 cells (I, U, L, Z)
*/
bool CheckMatching(Board &board);

Cell* TraverseChosenCells(Board &board, int &n);
Cell* TraversePathI(Cell c1, Cell c2, int &n);

bool AreMatchingI(Board &board, const Cell &c1, const Cell &c2);
bool AreMatchingL(Board &board, const Cell &c1, const Cell &c2, Cell &corner);
bool AreMatchingU(Board &board, Cell c1, Cell c2, Cell &corner1, Cell &corner2);
bool AreMatchingZ(Board &board, Cell c1, Cell c2, Cell &corner1, Cell &corner2);

bool TraverseTo(Board &board, int x, int y, int dest_x, int dest_y, List &path, int turn_type = -1);
#endif // GAME_BOARD_H_