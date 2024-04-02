#include "board.h"

#include <algorithm>
#include <random>
#include <chrono>

#include "linked_list.h"

const int search_dx[] = {-1, 0, 1, 0};
const int search_dy[] = {0, 1, 0, -1};
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

namespace Game {
    bool version_linked_list = false;
}

/**
 * Delete (deallocate) input arrays after merging.
*/
template<typename T>
static T* MergeArrays(int n1, T *arr1, int n2, T *arr2) {
    T *res = new T[n1 + n2];
    for (int i = 0; i < n1; ++i) {
        res[i] = arr1[i];
    }
    for (int i = 0; i < n2; ++i) {
        res[n1 + i] = arr2[i];
    }
    return res;
}

template<typename T>
static T* MergeArrays(int n1, T *arr1, int n2, T *arr2, int n3, T *arr3) {
    T *res = new T[n1 + n2 + n3];
    for (int i = 0; i < n1; ++i) {
        res[i] = arr1[i];
    }
    for (int i = 0; i < n2; ++i) {
        res[n1 + i] = arr2[i];
    }
    for (int i = 0; i < n3; ++i) {
        res[n1 + n2 + i] = arr3[i];
    }
    return res;
}

int NextInt(int a, int b) {
    return a + rng()%(b - a + 1);
}

/* Board */
Board& GenerateBoard(int m, int n, bool linked_list) {
    // Border cells
    m += 2;
    n += 2;

    Board *board;
    if (!linked_list) {
        board = new BoardInt(m, n);
    } else {
        board = new BoardLL(m, n);
    }
    // board->m = m;
    // board->n = n;
    // board->cells = new char*[m];
    // for (int i = 0; i < m; ++i) {
    //     board->cells[i] = new char[n]{};
    // }    

    // TODO Smart n
    char *chars = new char[(m - 2) * (n - 2)];
    for (int i = 0; i < (m - 2) * (n - 2); i += 2) {
        chars[i] = chars[i + 1] = 'A' + (char) NextInt(0, 25);
    }
    shuffle(chars, chars + (m - 2) * (n - 2), rng);

    for (int i = 1; i < m - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            // board->cells[i][j] = chars[(i - 1) * (n - 2) + j - 1]; 
            board->SetLetter(i, j, chars[(i - 1) * (n - 2) + j - 1]);
        }
    }

    delete[] chars;
    return *board;
}

void ShuffleBoard(Board &board) {
    int m = board.m, n = board.n;

    if (!Game::version_linked_list) {
        char *chars = new char[(m - 2) * (n - 2)];

        for (int i = 1; i < m - 1; ++i) {
            for (int j = 1; j < n - 1; ++j) {  
                chars[(i - 1) * (n - 2) + j - 1] = board.GetLetter(i, j); 
            }
        }
        shuffle(chars, chars + (m - 2) * (n - 2), rng);
        for (int i = 1; i < m - 1; ++i) {
            for (int j = 1; j < n - 1; ++j) {
                board.SetLetter(i, j, chars[(i - 1) * (n - 2) + j - 1]);
            }
        }

        delete[] chars;
    } else {
        int cnt = 0;
        for (int i = 1; i < m - 1; ++i) {
            for (int j = 1; j < n - 1; ++j) {  
                if (IsCellEmpty(board, i, j)) 
                    break;
                ++cnt;
            }
        }

        char *chars = new char[cnt];
        cnt = 0;
        for (int i = 1; i < m - 1; ++i) {
            for (int j = 1; j < n - 1; ++j) {
                if (IsCellEmpty(board, i, j)) 
                    break;
                chars[cnt++] = board.GetLetter(i, j);
            }
        }

        shuffle(chars, chars + cnt, rng);
        cnt = 0;
        for (int i = 1; i < m - 1; ++i) {
            for (int j = 1; j < n - 1; ++j) {
                if (IsCellEmpty(board, i, j))
                    break;
                board.SetLetter(i, j, chars[cnt++]);
            }
        }

        delete[] chars;

    }   
}

bool SuggestNextMove(Board &board, Cell &c1, Cell &c2) {
    Cell corner1, corner2;
    for (int i = 1; i < board.m - 1; ++i) {
        for (int j = 1; j < board.n - 1; ++j) {
            for (int x = i; x < board.m - 1; ++x) {
                for (int y = j; y < board.n - 1; ++y) {
                    Cell u(i, j), v(x, y);
                    if (u == v || IsCellEmpty(board, i, j) || IsCellEmpty(board, x, y)) 
                        continue;
                    
                    int n;
                    if (TraverseCells(board, u, v, n)) {
                        c1 = u;
                        c2 = v;
                        return true;
                    }
                    // if (AreMatchingI(board, u, v) || AreMatchingL(board, u, v, corner1) || AreMatchingU(board, u, v, corner1, corner2) || AreMatchingZ(board, u, v, corner1, corner2)) {
                    //     u = c1;
                    //     v = c2;
                    //     return true;
                    // }
                }
            }
        }
    }

    return false;
}

void DeleteBoard(Board &board) { 
    // for (int i = 0; i < board.m; ++i) {
    //     delete[] board.cells[i];
    // }
    // delete[] board.cells;
    delete &board;
}

bool IsCellEmpty(Board &board, int x, int y) {
    if (x < 0 || x >= board.m || y < 0 || y >= board.n)
        return false;

    return (board.GetLetter(x, y) == 0 || board.GetLetter(x, y) == ' ');
}

bool IsInside(Board &board, int x, int y) {
    return 1 <= x && x < board.m - 1 && 1 <= y && y < board.n - 1;
}

void BoardRemoveCell(Board &board, int x, int y) {
    board.RemoveCell(x, y);
    // TODO Remove linked list
}

void BoardGoToCell(Board &board, int x, int y) {
    board.cur_x = x;
    board.cur_y = y;
}

void AddChosenCell(Board &board, int x, int y) {
    if (board.chosen_cells.size >= 2) 
        return;

    Cell *cell = new Cell(x, y);
    AddToEnd(board.chosen_cells, cell);
}

// List size is at most 2. :)
// TODO Collapsing linked list
void RemoveChosenCell(Board &board, int x, int y) {
    Node *prv = nullptr;
    Node *cur = board.chosen_cells.head;

    while (cur) {
        Cell* cell = (Cell*) cur->data;
        
        if (cell->x == x && cell->y == y) {
            delete cell;
            RemoveAtPos(board.chosen_cells, cur, prv);
            break;
        }

        prv = cur;
        cur = cur->next;
    }
}

Cell *TraverseChosenCells2(Board &board, int &n) {
    if (board.chosen_cells.size < 2)
        return nullptr;

    Cell c1 = *(Cell*) board.chosen_cells.head->data;
    Cell c2 = *(Cell*) board.chosen_cells.tail->data;
    return TraverseCells(board, c1, c2, n);
}

Cell *TraverseCells(Board &board, Cell c1, Cell c2, int &n) {
    if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.GetLetter(c1.x, c1.y) != board.GetLetter(c2.x, c2.y)) 
        return nullptr; 

    Vertex**** trace = new Vertex***[board.m];
    for (int i = 0; i < board.m; ++i) {
        trace[i] = new Vertex**[board.n];
        for (int j = 0; j < board.n; ++j) {
            trace[i][j] = new Vertex*[5];
            for (int k = 0; k < 5; ++k) {
                trace[i][j][k] = new Vertex[3];
            }
        }
    }
    for (int i = 0; i < board.m; ++i) {
        for (int j = 0; j < board.n; ++j) {
            for (int k = 0; k < 5; ++k) {
                for (int l = 0; l < 3; ++l) {
                    trace[i][j][k][l] = {-1, -1, -1, -1};
                }
            }
        }
    }
    trace[c1.x][c1.y][4][0] = {-2, -2, -2, -2};
    TraverseMatching(board, trace, c1.x, c1.y, c2.x, c2.y, 4, 0);    

    // TODO Easy mode make this decreasing.
    for (int j = 0; j <= 2; ++j) {
        for (int i = 0; i < 4; ++i) {
            if (trace[c2.x][c2.y][i][j].x != -1) {
                int x = c2.x, y = c2.y;
                int turn_type = i, turns = j;
                int cnt = 1;
                while (trace[x][y][turn_type][turns].x != -2) {
                    int nx = trace[x][y][turn_type][turns].x;
                    int ny = trace[x][y][turn_type][turns].y;
                    int nturn_type = trace[x][y][turn_type][turns].turn_type;
                    int nturns = trace[x][y][turn_type][turns].turns;
                    x = nx; 
                    y = ny;
                    turn_type = nturn_type;
                    turns = nturns;
                    ++cnt;
                }
                n = cnt;

                Cell *path = new Cell[cnt];
                x = c2.x, y = c2.y;
                turn_type = i, turns = j;
                cnt = 0;
                while (trace[x][y][turn_type][turns].x != -2) {
                    path[cnt++] = Cell(x, y);
                    int nx = trace[x][y][turn_type][turns].x;
                    int ny = trace[x][y][turn_type][turns].y;
                    int nturn_type = trace[x][y][turn_type][turns].turn_type;
                    int nturns = trace[x][y][turn_type][turns].turns;
                    x = nx; 
                    y = ny;
                    turn_type = nturn_type;
                    turns = nturns;
                }
                path[cnt++] = Cell(x, y);

                for (int i = 0; i < n / 2; ++i) {
                    swap(path[i], path[n - i - 1]);
                }
                return path;
            }
       }
    }

    for (int i = 0; i < board.m; ++i) {
        for (int j = 0; j < board.n; ++j) {
            for (int k = 0; k < 5; ++k) {
                delete[] trace[i][j][k];
            }
            delete[] trace[i][j];
        }
        delete[] trace[i];
    }
    delete[] trace;
    return nullptr;
}

void TraverseMatching(Board &board, Vertex ****trace, int x, int y, int dest_x, int dest_y, int turn_type, int turns) {
    if (x == dest_x && y == dest_y) {
        return;
    }
    
    for (int i = 0; i < 4; ++i) {
        if (turn_type != 4 && abs(i - turn_type) == 2)
            continue;

        int nx = x + search_dx[i];
        int ny = y + search_dy[i];

        if (nx == dest_x && ny == dest_y || IsCellEmpty(board, nx, ny)) {
            int next_turns = turns + (turn_type != 4 && abs(i - turn_type) != 0);

            if (next_turns <= 2 && trace[nx][ny][i][next_turns].x == -1) {
                trace[nx][ny][i][next_turns] = {x, y, turn_type, turns};
                TraverseMatching(board, trace, nx, ny, dest_x, dest_y, i, next_turns);
            } 
        }
    }
}

// Cell* TraverseChosenCells(Board &board, int &n) {
//     if (board.chosen_cells.size < 2)
//         return nullptr;

//     Cell *c1 = (Cell*) board.chosen_cells.head->data;
//     Cell *c2 = (Cell*) board.chosen_cells.tail->data;

//     if (AreMatchingI(board, *c1, *c2)) {
//         return TraversePathI(*c1, *c2, n);
//     }
//     Cell corner1, corner2; 
//     if (AreMatchingL(board, *c1, *c2, corner1)) {
//         int n1, n2;
//         Cell *path1 = TraversePathI(*c1, corner1, n1);
//         Cell *path2 = TraversePathI(corner1, *c2, n2);
//         n = n1 + n2 - 1;
//         Cell *path = MergeArrays<Cell>(n1 - 1, path1, n2, path2);
//         delete[] path1;
//         delete[] path2;
//         return path;
//     }
//     if (AreMatchingU(board, *c1, *c2, corner1, corner2) || AreMatchingZ(board, *c1, *c2, corner1, corner2)) {
//         int n1, n2, n3;
//         Cell *path1 = TraversePathI(*c1, corner1, n1);
//         Cell *path2 = TraversePathI(corner1, corner2, n2);
//         Cell *path3 = TraversePathI(corner2, *c2, n3);
//         n = n1 + n2 + n3 - 2;
//         Cell *path = MergeArrays<Cell>(n1 - 1, path1, n2 - 1, path2, n3, path3);
//         delete[] path1;
//         delete[] path2;
//         delete[] path3;
//         return path;
//     }

//     return nullptr;
// }

// Cell* TraversePathI(Cell c1, Cell c2, int &n) {
//     if (c1.x == c2.x) {
//         bool flag = true;
//         n = c2.y - c1.y + 1;
//         if (c1.y > c2.y) {
//             n = c1.y - c2.y + 1;
//             flag = false;
//         }

//         Cell *path = new Cell[n];
//         if (flag) {
//             for (int y = c1.y; y <= c2.y; ++y) {
//                 path[y - c1.y] = Cell(c1.x, y);
//             }
//         } else {
//             for (int y = c2.y; y <= c1.y; ++y) {
//                 path[c1.y - y] = Cell(c1.x, y);
//             } 
//         }

//         return path;
//     } else if (c1.y == c2.y) {
//         bool flag = true;
//         n = c2.x - c1.x + 1;
//         if (c1.x > c2.x) {
//             flag = false;
//             n = c1.x - c2.x + 1;
//         }
        
//         // c1.x < x < c2.x
//         Cell *path = new Cell[n];
//         if (flag) {
//             for (int x = c1.x; x <= c2.x; ++x) {
//                 path[x - c1.x] = Cell(x, c1.y);
//             }
//         } else { 
//             for (int x = c2.x; x <= c1.x; ++x) {
//                 path[c1.x - x] = Cell(x, c1.y);
//             }
//         }
//         return path;
//     }
    
//     return nullptr;
// }

// bool AreMatchingI(Board &board, const Cell &c1, const Cell &c2) {
//     if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
//         return false;
//     }

//     if (c1.x == c2.x) {
//         int start = c1.y, end = c2.y;
//         if (c1.y > c2.y) {
//             start = c2.y;
//             end = c1.y;
//         }
        
//         for (int i = start + 1; i <= end - 1; ++i) {
//             if (!IsCellEmpty(board, c1.x, i)) {
//                 return false;
//             }         
//         }

//         return true;
//     } else if (c1.y == c2.y) {
//         int start = c1.x, end = c2.x;
//         if (c1.x > c2.x) {
//             start = c2.x;
//             end = c1.x;
//         }
        
//         for (int i = start + 1; i <= end - 1; ++i) {
//             if (!IsCellEmpty(board, i, c1.y)) {
//                 return false;
//             }         
//         }

//         return true;
//     }

//     return false;
// }

// bool AreMatchingL(Board &board, const Cell &c1, const Cell &c2, Cell &corner) {
//     if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
//         return false;
//     }
//     corner = Cell(c1.x, c2.y);
//     if (AreMatchingI(board, c1, corner) && AreMatchingI(board, corner, c2)) {
//         return true;
//     }
    
//     corner = Cell(c2.x, c1.y);
//     if (AreMatchingI(board, c1, corner) && AreMatchingI(board, corner, c2)) {
//         return true;
//     }

//     return false;
// }

// bool AreMatchingU(Board &board, Cell c1, Cell c2, Cell &corner1, Cell &corner2) {
//     if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
//         return false;
//     }

//     // Check U horizontally
//     for (int i = 0; i < 2; ++i) {
//         int x = c1.x;
//         int y = c1.y;
//         bool flag = false;
//         if (i == 0 && c1.x < c2.x || i == 1 && c1.x > c2.x) {
//             x = c2.x;
//             y = c2.y;
//             swap(c1, c2);
//             flag = true;
//         }
        
//         if (i == 0)
//             x++;
//         else 
//             x--;

//         while (IsCellEmpty(board, x, y)) {
//             if (AreMatchingL(board, Cell(x, y), c2, corner2)) {
//                 corner1 = Cell(x, y);
//                 if (flag) {
//                     swap(corner1, corner2);
//                 }
//                 return true;     
//             }

//             if (i == 0) 
//                 x++;
//             else 
//                 x--;   
//         }

//         if (flag) {
//             swap(c1, c2);
//         }
//     }

//     // Check U vertically
//     for (int i = 0; i < 2; ++i) {
//         int x = c1.x;
//         int y = c1.y;
//         bool flag = false;
//         if (i == 0 && c1.y < c2.y || i == 1 && c1.y > c2.y) {
//             x = c2.x;
//             y = c2.y;
//             swap(c1, c2);
//             flag = true;
//         }
        
//         if (i == 0)
//             y++;
//         else 
//             y--;

//         while (IsCellEmpty(board, x, y)) {
//             if (AreMatchingL(board, Cell(x, y), c2, corner2)) {
//                 corner1 = Cell(x, y);
//                 if (flag) {
//                     swap(corner1, corner2);
//                 }
//                 return true;     
//             }

//             if (i == 0) 
//                 y++;
//             else 
//                 y--;   
//         }

//         if (flag) {
//             swap(c1, c2);
//         }
//     }

//     return false;
// } 

// bool AreMatchingZ(Board &board, Cell c1, Cell c2, Cell &corner1, Cell &corner2) {
//     if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
//         return false;
//     }
//     // Check x
//     bool flag = false;
//     if (c1.x > c2.x) {
//         swap(c1, c2);
//         flag = true;
//     }

//     Cell cur(c1.x + 1, c1.y);
//     bool found = false;
//     while (cur.x < c2.x && IsCellEmpty(board, cur.x, cur.y)) {
//         if (AreMatchingL(board, cur, c2, corner2)) {
//             corner1 = cur;
//             found = true;
//             break;
//         }

//         ++cur.x;
//     }

//     if (found) {
//         if (flag) {
//             swap(corner1, corner2);
//         }
//         return true;
//     }

//     if (flag) {
//         swap(c1, c2);
//     }

//     // Check y
//     flag = false;
//     if (c1.y > c2.y) {
//         swap(c1, c2);
//         flag = true;
//     }

//     cur = Cell(c1.x, c1.y + 1);
//     found = false;
//     while (cur.y < c2.y && IsCellEmpty(board, cur.x, cur.y)) {
//         if (AreMatchingL(board, cur, c2, corner2)) {
//             corner1 = cur;
//             found = true;
//             break;
//         }

//         cur.y++;
//     }

//     if (found) {
//         if (flag)
//             swap(corner1, corner2);
//         return true;
//     }

//     return false;
// }