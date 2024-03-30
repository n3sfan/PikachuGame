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
const int kCellUnhovering = 5;
const int kCellMatchCorrect = 3;
const int kCellMatchIncorrect = 4;

Board& StartGame(int m, int n, bool linked_list) {
    Game::version_linked_list = linked_list;

    Board &board = GenerateBoard(m, n, linked_list); 
    DrawBoard(board);
    MoveToCell(board, 1, 1);

    // Check if a matching exists
    Cell c1, c2;
    while (!SuggestNextMove(board, c1, c2)) {
        ShuffleBoard(board);
        DrawBoard(board);
        bool found = false;
        for (int i = 1; i < board.m - 1; ++i) {
            for (int j = 1; j < board.n - 1; ++j) {
                if (!IsCellEmpty(board, i, j)) {
                    MoveToCell(board, i, j);
                    found = true; 
                    break;
                }
            }

            if (found)
                break;
        }
    }

    return board;
}

void StopGame(Board &board) {
    DeleteBoard(board);
}

void DrawCell(int x, int y, char c, int char_mode, int char_color, int background_color) { 
    int last_x, last_y; 
    ReturnCursorPos(last_x, last_y);
    GoToCursorPos(x, y);
    std::cout << (char)218;
    for (int i = 1; i < kCellWidth - 1; ++i) {
        std::cout << (char)196;
    }
    std::cout << (char)191;
    // cout << SetColor(0, background_color);
     for (int i = 1; i < kCellHeight - 1; ++i) {
        GoToCursorPos(x + i, y);
        std::cout << (char)179;
        cout << SetColor(0, 0, background_color);
        for (int j = 1; j < kCellWidth - 1; ++j) {
            if (i == (kCellHeight - 1) / 2 && j == (kCellWidth - 1) / 2) {
                std::cout << SetColor(0, char_mode, background_color) << c << SetColor(char_mode, kDefault, 0);
            } else {
                std::cout << " ";
            }
        }
        cout << SetColor(0, 0, kBackgroundDefault);
        std::cout << (char)179;
    }

    GoToCursorPos(x + kCellHeight - 1, y);
    std::cout << (char)192;
    for (int i = 1; i < kCellWidth - 1; ++i) {
        std::cout << (char)196; 
    }
    std::cout << (char)217;

    // Reset to original pos
    GoToCursorPos(last_x, last_y);
    std::cout << SetColor(kDefault);
}

void DrawEmptyCell(Cell cell) {
    // Draw empty cell and border
    int last_x, last_y; 
    ReturnCursorPos(last_x, last_y);

    int x, y;
    CellToPos(cell, x, y);
    GoToCursorPos(x, y);
    std::cout << SetColor(0, kDefault, kBackgroundDefault) << " ";
    for (int i = 1; i < kCellWidth - 1; ++i) {
        std::cout << " ";
    }
    std::cout << " ";

    for (int i = 1; i < kCellHeight - 1; ++i) {
        GoToCursorPos(x + i, y);
        std::cout << " ";
        for (int j = 1; j < kCellWidth - 1; ++j) {
            std::cout << " ";
        }
        std::cout << " ";
    }

    GoToCursorPos(x + kCellHeight - 1, y);
    std::cout << " ";
    for (int i = 1; i < kCellWidth - 1; ++i) {
        std::cout << " "; 
    }
    std::cout << " ";

    // Reset to original pos
    GoToCursorPos(last_x, last_y);
}

void DrawBackgroundCell(string filename, int file_x, int file_y, int x, int y) {
    ifstream fin(filename.c_str());
    
    fin.close();
}

void DrawMatching(const Cell *path, int n, bool clear) {
    int last_cursor_x, last_cursor_y;
    ReturnCursorPos(last_cursor_x, last_cursor_y);
    if (clear) {
        std::cout << SetColor(0, kDefault, kBackgroundDefault);
    } else {

    }

    for (int i = 0; i < n - 1; ++i) { 
        int x1, y1;
        CellToPos(path[i], x1, y1);
        int x2, y2;
        CellToPos(path[i + 1], x2, y2);

        bool hori = path[i + 1].x == path[i].x;
        int x = x1 + kCellHeight / 2, y = y1 + kCellWidth / 2;
        // Skip first (x, y) in every loop.
        while (x != x2 + kCellHeight / 2 || y != y2 + kCellWidth / 2) {
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
            if (!(i == n - 2 && x == x2 + kCellHeight / 2 && y == y2 + kCellWidth / 2)) {
                if (clear) {
                    std::cout << " ";
                } else {
                    std::cout << ("#");
                }
            }
        }
    }

    GoToCursorPos(last_cursor_x, last_cursor_y);
}

void DrawBoard(Board &board) {
    DrawBoard(board, 1);
}

void DrawBoard(Board &board, int x) {
    for (int i = x; i < board.m - 1; ++i) {
        DrawRow(board, i, 1);
    }
}

void DrawRow(Board &board, int x, int y) {
    for (int j = y; j < board.n - 1; ++j) {
        if (IsCellEmpty(board, x, j)) {
            //GameRemoveCell(board, Cell(x, j));
            DrawEmptyCell(Cell(x, j));
        } else {
            DrawCell(x * kCellHeight, j * kCellWidth, board.GetLetter(x, j));
        }
    }
}

// TODO Buggy, infinite recursion
void FindNextUnmatchedCell(Board &board, int x, int y, int key_pressed, int &next_x, int &next_y) {
    if (key_pressed == kKeyUp || key_pressed == kKeyDown) {
        int d = (key_pressed == kKeyDown ? 1 : -1);
        bool cycled = false;

        // This loop runs once in LL.
        for (int tx = x + d; ; tx += d) {
            // border cell
            if (tx == 0 || tx == board.GetGameRows() - 1) {
                if (cycled)
                    break;

                cycled = true;
                if (tx == 0)
                    tx = board.GetGameRows() - 2;
                else   
                    tx = 1;
            }

            int d2 = 0;
            int n = board.GetGameRowSize(x);
            while (1 <= y - d2 || y + d2 <= n - 1) {
                if (1 <= y - d2 && !IsCellEmpty(board, tx, y - d2)) {
                    next_x = tx;
                    next_y = y - d2;
                    return;
                }
                if (y + d2 <= n - 1 && !IsCellEmpty(board, tx, y + d2)) {
                    next_x = tx;
                    next_y = y + d2;
                    return;
                }
                ++d2;
            }
        }
    } else {
        int d = (key_pressed == kKeyRight ? 1 : -1);
        bool cycled = false;

        for (int ty = y + d; ; ty += d) {
            int n = board.GetGameRowSize(x);
            // border cell
            if (ty == 0 || ty == n - 1) {
                if (cycled)
                    break;

                cycled = true;
                if (ty == 0)
                    ty = n - 2;
                else   
                    ty = 1;
            }

            int d2 = 0;
            // This loop runs once in LL.
            while (1 <= x - d2 || x + d2 <= board.m - 1) {
                if (1 <= x - d2 && !IsCellEmpty(board, x - d2, ty)) {
                    next_x = x - d2;
                    next_y = ty;
                    return; 
                }
                if (x + d2 <= board.m - 1 && !IsCellEmpty(board, x + d2, ty)) {
                    next_x = x + d2;
                    next_y = ty;
                    return;
                }
                ++d2;
            }
        }
    }
    
    // next_x = x;
    // next_y = y;

    // if (key_pressed == kKeyUp || key_pressed == kKeyDown) { 
    //     int d = (key_pressed == kKeyUp ? -1 : 1);
    //     next_x = x + d;
    //     while (IsInside(board, next_x, next_y) && IsCellEmpty(board, next_x, next_y)) 
    //         next_x += d;

    //     // at least 1 cell, which is (x, y)
    //     if (!IsInside(board, next_x, next_y)) {
    //         if (key_pressed == kKeyDown) {
    //             FindNextUnmatchedCell(board, 0, y, kKeyDown, next_x, next_y);
    //         } else {
    //             FindNextUnmatchedCell(board, board.m - 1, y, kKeyUp, next_x, next_y);
    //         }
    //     }
    // } else { 
    //     // key right or left 
    //     int d = (key_pressed == kKeyRight ? 1 : -1);
    //     next_y += d;
    //     while (IsInside(board, next_x, next_y) && IsCellEmpty(board, next_x, next_y)) 
    //         next_y += d;

    //     // at least 1 cell, which is (x, y)
    //     if (!IsInside(board, next_x, next_y)) {
    //         if (key_pressed == kKeyRight) {
    //             FindNextUnmatchedCell(board, x, 0, kKeyRight, next_x, next_y);
    //         } else {
    //             FindNextUnmatchedCell(board, x, board.n - 1, kKeyLeft, next_x, next_y);
    //         }
    //     }
    // }
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
            BoardGoToCell(board, x, y);
            if (board.cur_x != last_x || board.cur_y != last_y) {
                // std::cout << board.cur_x << " " << board.cur_y << "\n";
                NotifyCell(board, last_x, last_y, kCellUnhovering);
                NotifyCell(board, board.cur_x, board.cur_y, kCellHovering);
            }
            break;
        case kKeyEnter:
            ChooseCell(board, board.cur_x, board.cur_y);
            break;
        case kKeyEsc:
            StopGame(board);
            break;
        case kKeyBackspace:
            // TOOD Deselect cell
            // TODO REMOVE THIS LINE ON RELEASE
            // GameRemoveCell(board, Cell(x, y));
            ShuffleBoard(board);
            DrawBoard(board);
            break;
        default:
            break;
    }
}

// TODO FIX
void ChooseCell(Board &board, int x, int y) {     
    bool (*pred)(void*, void*) = [](void *element, void *value) -> bool {
        return *((Cell*) element) == *((Cell*) value);
    };
    Cell tmp(x, y);
    if (board.chosen_cells.size == 2 || ListContains(board.chosen_cells, &tmp, pred))
        return;
    
    AddChosenCell(board, x, y);
    NotifyCell(board, x, y, kCellChosen);

    if (board.chosen_cells.size == 2) {
        int n;
        Cell* path = TraverseChosenCells2(board, n);
        Cell c1 = *(Cell*) board.chosen_cells.head->data;
        Cell c2 = *(Cell*) board.chosen_cells.tail->data;
        // Linked list
        Cell c3 = c2;
        RemoveChosenCell(board, c1.x, c1.y);
        RemoveChosenCell(board, c2.x, c2.y); 

        bool flag1 = board.GetLetter(c1.x, 2) == ' ';
        bool flag2 = false;
        
        if (path) {
            // Remove cells in the board first, to avoid moving to deleted cell bug.
            if (!Game::version_linked_list) {
                BoardRemoveCell(board, c1.x, c1.y);
                BoardRemoveCell(board, c2.x, c2.y);
            } else {
                BoardRemoveCell(board, c1.x, c1.y);
                c3 = c2;
                if (flag1 && c1.x < c2.x) 
                    --c3.x;
                else if (c1.x == c2.x && c1.y < c2.y)
                    --c3.y;
                flag2 = board.GetLetter(c2.x, 2) == ' ';
                BoardRemoveCell(board, c3.x, c3.y);
            }

            DrawMatching(path, n, false);
            Sleep(1000);
            DrawMatching(path, n, true);

            // int next_x, next_y;
            // int keys[] = {kKeyRight, kKeyLeft, kKeyUp, kKeyDown};
            // bool found = false;
            // for (int i = 0; i < 4; ++i) {
            //     FindNextUnmatchedCell(board, x, y, keys[i], next_x, next_y);

            //     if (!IsCellEmpty(board, next_x, next_y)) {
            //         MoveToCell(board, next_x, next_y);
            //         found = true;
            //         break;
            //     }
            // }
        } else {
            // Sound
        }

        NotifyCell(board, c1.x, c1.y, kCellUnchosen);
        NotifyCell(board, c2.x, c2.y, kCellUnchosen);
       
        if (path) {
            if (!Game::version_linked_list) {
                GameRemoveCell(board, c1);
                GameRemoveCell(board, c2);
            } else {
                GameRemoveCell(board, c1, flag1);
                GameRemoveCell(board, c2, flag2);
            }
            
            bool found = false;
            for (int i = 1; i < board.m - 1; ++i) {
                for (int j = 1; j < board.n - 1; ++j) {
                    if (!IsCellEmpty(board, i, j)) {
                        MoveToCell(board, i, j);
                        found = true; 
                        break;
                    }
                }

                if (found)
                    break;
            }
        } else {
            NotifyCell(board, x, y, kCellHovering);
        }

        delete[] path;
    }

    // Check if a matching exists
    Cell c1, c2;
    while (!SuggestNextMove(board, c1, c2)) {
        ShuffleBoard(board);
        DrawBoard(board);
        bool found = false;
        for (int i = 1; i < board.m - 1; ++i) {
            for (int j = 1; j < board.n - 1; ++j) {
                if (!IsCellEmpty(board, i, j)) {
                    MoveToCell(board, i, j);
                    found = true; 
                    break;
                }
            }

            if (found)
                break;
        }
    }
}

void GameRemoveCell(Board &board, Cell cell, bool redraw_rows) {
    // BoardRemoveCell(board, cell.x, cell.y);

    // If linked list:
    //  redraw only 1 column from (x, y) if second element not empty.
    //  otherwise redraw from row x.
    if (!Game::version_linked_list) {
        DrawEmptyCell(cell);
    } else {
        if (redraw_rows) {
            DrawBoard(board, cell.x);
        } else {
            DrawRow(board, cell.x, cell.y);
        }
    }
}

/**
 * Redraw cell after removing in Board.
*/
void GameRemoveCell(Board &board, Cell cell) {
    GameRemoveCell(board, cell, false);
}

void MoveToCell(Board &board, int x, int y) {
    BoardGoToCell(board, x, y);
    GoToCursorPos(x * kCellHeight, y * kCellWidth);
    NotifyCell(board, x, y, kCellHovering);
}

void CellToPos(const Cell &c, int &x, int &y) {
    x = c.x * kCellHeight;
    y = c.y * kCellWidth;
}

/**
 * Draw decoration and play audio.
 * TODO Reset color mode (bold)
*/
void NotifyCell(Board &board, int x, int y, int state) {
    Cell cur(x, y);
    bool (*pred)(void*, void*) = [](void *element, void *value) -> bool {
        return *((Cell*) element) == *((Cell*) value);
    };
   
    switch (state) {
        case kCellChosen:
            DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), kBold, kDefault, kBackgroundBlue);
            break;
        case kCellUnchosen:
            DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), 0, kDefault, kBackgroundDefault); 
            break;
        case kCellHovering:
            if (!ListContains(board.chosen_cells, &cur, pred)) {
                DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), 0, kDefault, kBackgroundGreen);
            }
            break;
        case kCellUnhovering:
            if (!ListContains(board.chosen_cells, &cur, pred)) {
                DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), 0, kDefault, kBackgroundDefault);
            }

            break;
        default:
            break;
    }
}