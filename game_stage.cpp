#define WIN32_LEAN_AND_MEAN

#include "game_stage.h"

#include <windows.h>

#include <iostream>
#include <fstream>

#include "draw_console.h"
#include "board.h"

using namespace std;

const int kCellUnchosen = 0;
const int kCellChosen = 1;
const int kCellHovering = 2;
const int kCellMatchCorrect = 3;
const int kCellMatchIncorrect = 4;

Board& StartGame(int m, int n) {
    Board &board = GenerateBoard(m, n); 
    DrawBoard(board);
    MoveToCell(board, 1, 1);
    NotifyCell(board, 1, 1, kCellHovering);
    return board;
}

void StopGame(Board &board) {
    DeleteBoard(board);
}

void DrawCell(int x, int y, char c, int char_mode, int char_color, int background_color) { 
    int last_x, last_y; 
    ReturnCursorPos(last_x, last_y);
    GoToCursorPos(x, y);
    std::cout << SetColor(0, background_color) << " ";
    for (int i = 1; i < kCellWidth - 1; ++i) {
        std::cout << "-";
    }
    std::cout << " ";

    for (int i = 1; i < kCellHeight - 1; ++i) {
        GoToCursorPos(x + i, y);
        std::cout << "|";
        for (int j = 1; j < kCellWidth - 1; ++j) {
            if (i == (kCellHeight - 1) / 2 && j == (kCellWidth - 1) / 2) {
                std::cout << SetColor(0, char_mode, background_color) << c << SetColor(char_mode, kDefault, 0);
            } else {
                std::cout << " ";
            }
        }
        std::cout << "|";
    }

    GoToCursorPos(x + kCellHeight - 1, y);
    std::cout << " ";
    for (int i = 1; i < kCellWidth - 1; ++i) {
        std::cout << "-"; 
    }
    std::cout << " ";

    // Reset to original pos
    GoToCursorPos(last_x, last_y);

    std::cout << SetColor(kDefault);
}

void DrawBackgroundCell(string filename, int file_x, int file_y, int x, int y) {
    ifstream fin(filename.c_str());
    
    fin.close();
}

void DrawMatching(const Cell *path, int n) {
    int last_cursor_x, last_cursor_y;
    ReturnCursorPos(last_cursor_x, last_cursor_y);

    for (int i = 0; i < n - 1; ++i) { 
        int x1, y1;
        CellToPos(path[i], x1, y1);
        int x2, y2;
        CellToPos(path[i + 1], x2, y2);

        bool hori = path[i + 1].x == path[i].x;
        int x = x1 + kCellHeight / 2, y = y1 + kCellWidth / 2;
        // Skip first (x, y) in every loop.
        while (x != x2 + kCellHeight / 2 && y != y2 + kCellWidth / 2) {
            // move horizontally
            if (hori) { 
                if (path[i].y < path[i + 1].y) {
                    ++y;
                } else {
                    --y;
                }
            } else {
                // move vertically
                if (path[i].x < path[i + 1].x) {
                    ++x;
                } else {
                    --x;
                }
            }

            GoToCursorPos(x, y);
            // Do not print on last character of last cell on path
            if (!(i == n - 1 && x == x2 + kCellHeight / 2 && y == y2 + kCellWidth / 2)) {
                std::cout << (hori ? "-" : "|");
            }
        }

//        if (i < n - 1) {
//            int x2, y2;
//            CellToPos(path[i + 1], x2, y2);

//            bool next_hori = true;
//            if (path[i + 1].x - path[i].x == -1 || path[i + 1].x - path[i].x == 1) {
//                next_hori = false;
//            }

//            while (x != x2 + kCellHeight / 2 && y != y2 + kCellWidth / 2) {
//                if (next_hori) {
//                    if (path[i].y < path[i + 1].y) {
//                        ++y;
//                    } else {
//                        --y;
//                    }
//                } else {
//                    if (path[i].x < path[i + 1].x) {
//                        ++x;
//                    } else {
//                        --x;
//                    }
//                }

//                GoToCursorPos(x, y);
//                std::cout << (next_hori ? "-" : "|");
//            }
//        }
//        hori = next_hori;
    }
    //Sleep(1000);

    GoToCursorPos(last_cursor_x, last_cursor_y);
}

void DrawBoard(Board &board) {
    for (int i = 1; i < board.m - 1; ++i) {
        for (int j = 1; j < board.n - 1; ++j) {
            DrawCell(i * kCellHeight, j * kCellWidth, board.cells[i][j]);
        }
    }
}

void FindNextUnmatchedCell(Board &board, int x, int y, int key_pressed, int &next_x, int &next_y) {
    next_x = x;
    next_y = y;

    if (key_pressed == kKeyUp || key_pressed == kKeyDown) { 
        int d = (key_pressed == kKeyUp ? -1 : 1);
        next_x = x + d;
        while (IsCellEmpty(board, next_x, next_y)) 
            next_x += d;

        // at least 1 cell, which is (x, y)
        if (!IsInside(board, next_x, next_y)) {
            if (key_pressed == kKeyDown) {
                FindNextUnmatchedCell(board, 0, y, kKeyDown, next_x, next_y);
            } else {
                FindNextUnmatchedCell(board, board.m - 1, y, kKeyUp, next_x, next_y);
            }
        }
    } else { 
        // key right or left 
        int d = (key_pressed == kKeyRight ? 1 : -1);
        next_y += d;
        while (IsCellEmpty(board, next_x, next_y)) 
            next_y += d;
        
        // at least 1 cell, which is (x, y)
        if (!IsInside(board, next_x, next_y)) {
            if (key_pressed == kKeyRight) {
                FindNextUnmatchedCell(board, x, 0, kKeyRight, next_x, next_y);
            } else {
                FindNextUnmatchedCell(board, x, board.n - 1, kKeyLeft, next_x, next_y);
            }
        }
    }
}

void OnKeyPressed(Board &board, char key) {
    int last_x = board.cur_x, last_y = board.cur_y;

    switch (key) {
        case kKeyUp:
        case kKeyDown:
        case kKeyRight:
        case kKeyLeft:
            int x, y;
            FindNextUnmatchedCell(board, last_x, last_y, key, x, y);
            GoToBoardCell(board, x, y);
            if (board.cur_x != last_x || board.cur_y != last_y) {
                // std::cout << board.cur_x << " " << board.cur_y << "\n";
                NotifyCell(board, last_x, last_y, kCellUnchosen);
                NotifyCell(board, board.cur_x, board.cur_y, kCellHovering);
            }
            break;
        case kKeyEnter:
            ChooseCell(board, board.cur_x, board.cur_y);
            break;
        case kKeyEsc:
            StopGame(board);
            break;
        default:
            break;
    }
}

void ChooseCell(Board &board, int x, int y) {     
    AddChosenCell(board, x, y);
    NotifyCell(board, x, y, kCellChosen);

    if (board.chosen_cells.size == 2) {
        if (CheckMatching(board)) {
            // TODO DrawMatching(path);

            int cx, cy;
            CellToPos(Cell(x, y), cx, cy);
            // TODO Erase cell
            DrawCell(cx, cy, 0, 0, kDefault, 0);
            // RemoveCell(board, x, y);
        } else {
            // Sound
        }
    }
}

void MoveToCell(Board &board, int x, int y) {
    GoToBoardCell(board, x, y);
    GoToCursorPos(x * kCellHeight, y * kCellWidth);
}

void CellToPos(const Cell &c, int &x, int &y) {
    x = c.x * kCellHeight;
    y = c.y * kCellWidth;
}

/**
 * Draw decoration and play audio.
*/
void NotifyCell(Board &board, int x, int y, int state) {
    switch (state) {
        case kCellChosen:
            DrawCell(x * kCellHeight, y * kCellWidth, board.cells[x][y], kBold, kWhite, kBackgroundBlue);
            break;
        case kCellUnchosen:
            DrawCell(x * kCellHeight, y * kCellWidth, board.cells[x][y], 0, kWhite, kBackgroundBlack); 
            break;
        case kCellHovering:
            DrawCell(x * kCellHeight, y * kCellWidth, board.cells[x][y], 0, kWhite, kBackgroundGreen);
            break;
        default:
            break;
    }
}