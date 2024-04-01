#define WIN32_LEAN_AND_MEAN

#include "game_stage.h"

#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "draw_console.h"
#include "board.h"

using namespace std;

const int kCellUnchosen = 0;
const int kCellChosen = 1;
const int kCellHovering = 2;
const int kCellUnhovering = 5;
const int kCellMatchCorrect = 3;
const int kCellMatchIncorrect = 4;
const int kCellHint = 6;
const int kMaxTop = 10;

namespace Game {
    chrono::_V2::system_clock::time_point score;
    int m, n;
    string background_image;
}

Board& StartGame(int m, int n, bool linked_list) {
    Game::version_linked_list = linked_list;
    Game::score = chrono::system_clock::now();
    Game::m = m;
    Game::n = n;
    Game::background_image = string("background") + to_string(1 + rng()%3) + ".txt";

    Board &board = GenerateBoard(m, n, linked_list); 

    // draw background: diffusion effect
    Cell *bg_cells = new Cell[90*90];
    for (int i = 0; i < 90; ++i) {
        for (int j = 0; j < 90; ++j) {
            bg_cells[i * 90 + j] = Cell(i, j);
        }
    }
    shuffle(bg_cells, bg_cells + 90*90, rng);
    for (int k = 0; k < 90*90; ++k) {
        int i = bg_cells[k].x, j = bg_cells[k].y;
        DrawBackgroundCell(Game::background_image, i, j, i, j, 1, 1);
    }
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

bool IsGameFinished(Board &board) {
    for (int i = 1; i < board.m - 1; ++i) {
        for (int j = 1; j < board.n - 1; ++j) {
            if (!IsCellEmpty(board, i, j)) {
                return false;
            }
        }
    }
    return true;
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

void DrawEmptyCell(Cell cell, int char_mode, int char_color, int background_color, bool clear) {
    // Draw empty cell and border
    int last_x, last_y; 
    ReturnCursorPos(last_x, last_y);

    int x, y;
    CellToPos(cell, x, y);
    std::cout << SetColor(char_mode, char_color, background_color);

    if (clear) { 
        GoToCursorPos(x, y);
        std::cout << " ";
        for (int i = 1; i < kCellWidth - 1; ++i) {
            std::cout << " ";
        }
        std::cout << " ";
    }

    for (int i = 1; i < kCellHeight - 1; ++i) {
        GoToCursorPos(x + i, y);
        if (clear) 
            std::cout << " ";
        for (int j = 1; j < kCellWidth - 1; ++j) {
            std::cout << " ";
        }
        if (clear)
            std::cout << " ";
    }

    if (clear) {
        GoToCursorPos(x + kCellHeight - 1, y);
        std::cout << " ";
        for (int i = 1; i < kCellWidth - 1; ++i) {
            std::cout << " "; 
        }
        std::cout << " ";
    }

    if (cell.x >= 1 && cell.y >= 1)
        DrawBackgroundCell(Game::background_image, x, y, x, y, kCellHeight, kCellWidth);
    // Reset to original pos
    GoToCursorPos(last_x, last_y);
    cout << SetColor(0, kDefault, kBackgroundDefault);
}

void DrawBackgroundCell(string filename, int file_x, int file_y, int x, int y, int h, int w) {
    ifstream fin(filename.c_str());
    string line = "";
    for (int i = 0; i <= file_x; ++i) {
        if (!std::getline(fin, line))
            return;
    }

    int last_x, last_y;
    ReturnCursorPos(last_x, last_y);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if (file_y + j >= line.size()) {
                break;
            }
            GoToCursorPos(x + i, y + j);
            std::cout << line[file_y + j];
        }

        if (!getline(fin, line))
            break;
    }
    fin.close();
    GoToCursorPos(last_x, last_y);
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
                    std::cout << SetColor(0, kDefault, kBackgroundDefault) << " ";
                } else {
                    std::cout << SetColor(0, kGreen, kBackgroundDefault) << "#";
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
            DrawCell(x * kCellHeight, j * kCellWidth, board.GetLetter(x, j), kBold, kDefault);
        }
    }
}

// TODO Move in the original dir first, then check later
void FindNextUnmatchedCell(Board &board, int x, int y, int key_pressed, int &next_x, int &next_y) {
    if (key_pressed == kKeyUp || key_pressed == kKeyDown) {
        int d = (key_pressed == kKeyDown ? 1 : -1);
        bool cycled = false;

        // if (x + d < 1) {
        //     next_x = board.m - 2;
        // } else if (x + d >= board.m - 1) {
        //     next_x = 1;
        // } else {
        //     next_x = x + d;
        // }
        // next_y = y;

        // This loop runs once in LL.
        // prioritize searching original direction first
        // for (int tx = x + d; ; tx += d) {
        //     // border cell
        //     if (tx == 0 || tx == board.GetGameRows() - 1) {
        //         if (cycled)
        //             break;

        //         cycled = true;
        //         if (tx == 0)
        //             tx = board.GetGameRows() - 2;
        //         else   
        //             tx = 1;
        //     }

        //     int d2 = 0;
        //     int n = board.GetGameRowSize(x);
        //     if (1 <= y - d2 && !IsCellEmpty(board, tx, y - d2)) {
        //         next_x = tx;
        //         next_y = y - d2;
        //         return;
        //     }
        //     if (y + d2 <= n - 1 && !IsCellEmpty(board, tx, y + d2)) {
        //         next_x = tx;
        //         next_y = y + d2;
        //         return;
        //     }
        // }
        // Not run in LL.
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
            while (1 <= y - d2 || y + d2 < n - 1) {
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

        // next_x = x;
        // if (y + d < 1) {
        //     next_y = board.m - 2;
        // } else if (y + d >= board.m - 1) {
        //     next_y = 1;
        // } else {
        //     next_y = y + d;
        // }
        // prioritize searching original direction first
        // for (int ty = y + d; ; ty += d) {
        //     int n = board.GetGameRowSize(x);
        //     // border cell
        //     if (ty == 0 || ty == n - 1) {
        //         if (cycled)
        //             break;

        //         cycled = true;
        //         if (ty == 0)
        //             ty = n - 2;
        //         else   
        //             ty = 1;
        //     }

        //     int d2 = 0;
        //     if (1 <= x - d2 && !IsCellEmpty(board, x - d2, ty)) {
        //         next_x = x - d2;
        //         next_y = ty;
        //         return; 
        //     }
        //     if (x + d2 <= board.m - 1 && !IsCellEmpty(board, x + d2, ty)) {
        //         next_x = x + d2;
        //         next_y = ty;
        //         return;
        //     }
        // }
        
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
            while (1 <= x - d2 || x + d2 < board.m - 1) {
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
}

void OnKeyPressed(Board &board, char key) {
    int last_x = board.cur_x, last_y = board.cur_y;
    Cell c1, c2;

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
        case 'h':
            if (SuggestNextMove(board, c1, c2)) {
                while (board.chosen_cells.head) {
                    Cell c = *(Cell*) board.chosen_cells.head->data;
                    NotifyCell(board, c.x, c.y, kCellUnchosen);
                    RemoveChosenCell(board, c.x, c.y);
                }

                AddChosenCell(board, c1.x, c1.y);
                AddChosenCell(board, c2.x, c2.y);
                NotifyCell(board, c1.x, c1.y, kCellHint);
                NotifyCell(board, c2.x, c2.y, kCellHint);
            }
            break;
        default:
            break;
    }
}

void ChooseCell(Board &board, int x, int y) {     
    bool (*pred)(void*, void*) = [](void *element, void *value) -> bool {
        return *((Cell*) element) == *((Cell*) value);
    };
    Cell tmp(x, y);
    // if (board.chosen_cells.size == 2 || ListContains(board.chosen_cells, &tmp, pred))
    //     return;

    if (board.chosen_cells.size < 2 && !IsCellEmpty(board, x, y)) {
        AddChosenCell(board, x, y);
        NotifyCell(board, x, y, kCellChosen);
    }

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
            Sleep(700);
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
            NotifyCell(board, c1.x, c1.y, kCellMatchIncorrect);
            NotifyCell(board, c2.x, c2.y, kCellMatchIncorrect);
            Sleep(300);
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

    // Check if game is finished
    if (IsGameFinished(board)) {
        StopGame(board);
        EraseScreen();
        Sleep(800L);
        DrawEndingScoreScreen();
        return;
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
 * Redraw cell after removing in Board. The last call.
*/
void GameRemoveCell(Board &board, Cell cell) {
    GameRemoveCell(board, cell, false);
}

void MoveToCell(Board &board, int x, int y) {
    if (board.cur_x != 0 && board.cur_y != 0)
        NotifyCell(board, board.cur_x, board.cur_y, kCellUnhovering);
    
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
            DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), kYellow, kDefault, kBackgroundRed);
            break;
        case kCellUnchosen:
            if (IsCellEmpty(board, x, y))
                DrawEmptyCell(Cell(x, y));
            else 
                DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), 0, kDefault, kBackgroundDefault); 
            break;
        case kCellHovering:
            if (IsCellEmpty(board, x, y))
                DrawEmptyCell(Cell(x, y), 0, kDefault, kBackgroundGreen, false);
            else if (!ListContains(board.chosen_cells, &cur, pred)) {
                DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), 0, kDefault, kBackgroundGreen);
            }
            break;
        case kCellUnhovering:
            if (IsCellEmpty(board, x, y))
                DrawEmptyCell(Cell(x, y));
            else if (!ListContains(board.chosen_cells, &cur, pred)) {
                DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), 0, kDefault, kBackgroundDefault);
            }

            break;
        case kCellHint:
            DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), kBold, kDefault, kBackgroundYellow);
            break;
        case kCellMatchIncorrect:
            DrawCell(x * kCellHeight, y * kCellWidth, board.GetLetter(x, y), kBlinking, kDefault, kBackgroundDefault);
            break;
        default:
            break;
    }
}

/* Ending Score */
void DrawEndingScoreScreen() {
    // TODO Play audio
    // TODO Draw background
    int col = 30;

    GoToCursorPos(30, col);
    auto end = chrono::system_clock::now();
    int player_time = chrono::duration<double, milli>(end-Game::score).count() / 1000;
    std::cout << "Finished! You've finished in " << player_time << " seconds!\n"; 
    GoToCursorPos(31, col);
    // Make cursor visible
    std::cout << "\x1b[?25h";

    std::cout << "Enter your name: ";
    string player_name;
    std::getline(cin, player_name);
    while (player_name.size() >= 16) {
        GoToCursorPos(32, col);
        std::cout << "Name must be <= 16 characters!\n";
        GoToCursorPos(31, col + string("Enter your name: ").length());
        
        for (int i = 0; i < player_name.size(); ++i) {
            std::cout << " ";
        }
        GoToCursorPos(31, col + string("Enter your name: ").length());
        getline(cin, player_name);
    }

    stringstream ss;
    ss << Game::m << "x" << Game::n;
    ss << "_";
    if (!Game::version_linked_list) {
        ss << "standard";
    } else {
        ss << "collapsing";
    }
    ss << ".txt";
    string filename = ss.str();

    ifstream fin(filename.c_str());
    int lines = 0;
    string line;
    while (std::getline(fin, line)) 
        ++lines;
    fin.close();


    Player *players = new Player[lines + 1];
    int i = 0;
    string tmp = "";
    bool flag = false;
    
    fin.open(filename.c_str());
    while (std::getline(fin, line)) {
        int p = line.find_last_of('-'); 
        string cur_name = line.substr(0, p - 1); 
        int cur_time = stoi(line.substr(p + 1));

        if (cur_time >= player_time && !flag) {
            players[i++] = Player{player_name, player_time};
            flag = true;
        } 
        players[i++] = Player{cur_name, cur_time};  
    } 
    fin.close();
    if (!flag) {
        players[i++] = Player{player_name, player_time};
    }

    if (i > kMaxTop) {
        --i;
    }

    ofstream fout(filename.c_str());
    for (int j = 0; j < i; ++j) {
        fout << players[j].name << " - " << players[j].score << "\n";
    }
    fout.close();

    delete[] players;

    GoToCursorPos(32, col);
    // erase whole line
    std::cout << "\x1b[2k";
    std::cout << "Saved player " << player_name << "!\n";
    Sleep(800L);

    EraseScreen();
    // TODO Go To menu
}