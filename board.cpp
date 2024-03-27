#include "board.h"

#include <algorithm>
#include <random>
#include <chrono>

#include "linked_list.h"

const int search_dx[] = {-1, 0, 1, 0};
const int search_dy[] = {0, 1, 0, -1};
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

struct CellInfo {
    Cell c;
    bool flag = false;
    int d = 0;  
};

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
Board& GenerateBoard(int m, int n) {
    // Border cells
    m += 2;
    n += 2;

    Board *board = new Board;
    board->m = m;
    board->n = n;
    board->cells = new char*[m];
    for (int i = 0; i < m; ++i) {
        board->cells[i] = new char[n]{};
    }    

    // TODO Smart gen
    char *chars = new char[(m - 2) * (n - 2)];
    for (int i = 0; i < (m - 2) * (n - 2); i += 2) {
        chars[i] = chars[i + 1] = 'A' + (char) NextInt(0, 25);
    }
    shuffle(chars, chars + (m - 2) * (n - 2), rng);

    for (int i = 1; i < m - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            board->cells[i][j] = chars[(i - 1) * (n - 2) + j - 1]; 
        }
    }

    delete[] chars;
    return *board;
}

void ShuffleBoard(Board &board) {
    int m = board.m, n = board.n;
    char *chars = new char[(m - 2) * (n - 2)];

    for (int i = 1; i < m - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            chars[(i - 1) * (n - 2) + j - 1] = board.cells[i][j]; 
        }
    }
    shuffle(chars, chars + (m - 2) * (n - 2), rng);
    for (int i = 1; i < m - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            board.cells[i][j] = chars[(i - 1) * (n - 2) + j - 1]; 
        }
    }

    delete[] chars;
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

                    if (AreMatchingI(board, u, v) || AreMatchingL(board, u, v, corner1) || AreMatchingU(board, u, v, corner1, corner2) || AreMatchingZ(board, u, v, corner1, corner2)) {
                        u = c1;
                        v = c2;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void DeleteBoard(Board &board) { 
    for (int i = 0; i < board.m; ++i) {
        delete[] board.cells[i];
    }
    delete[] board.cells;
    delete &board;
}

bool IsCellEmpty(Board &board, int x, int y) {
    if (x < 0 || x >= board.m || y < 0 || y >= board.n)
        return false;

    return (board.cells[x][y] == 0 || board.cells[x][y] == ' ');
}

bool IsInside(Board &board, int x, int y) {
    return 1 <= x && x < board.m - 1 && 1 <= y && y < board.n - 1;
}

void BoardRemoveCell(Board &board, int x, int y) {
    board.cells[x][y] = ' ';
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

Cell* TraverseChosenCells(Board &board, int &n) {
    if (board.chosen_cells.size < 2)
        return nullptr;

    Cell *c1 = (Cell*) board.chosen_cells.head->data;
    Cell *c2 = (Cell*) board.chosen_cells.tail->data;

    if (AreMatchingI(board, *c1, *c2)) {
        return TraversePathI(*c1, *c2, n);
    }
    Cell corner1, corner2; 
    if (AreMatchingL(board, *c1, *c2, corner1)) {
        int n1, n2;
        Cell *path1 = TraversePathI(*c1, corner1, n1);
        Cell *path2 = TraversePathI(corner1, *c2, n2);
        n = n1 + n2 - 1;
        Cell *path = MergeArrays<Cell>(n1 - 1, path1, n2, path2);
        delete[] path1;
        delete[] path2;
        return path;
    }
    if (AreMatchingU(board, *c1, *c2, corner1, corner2) || AreMatchingZ(board, *c1, *c2, corner1, corner2)) {
        int n1, n2, n3;
        Cell *path1 = TraversePathI(*c1, corner1, n1);
        Cell *path2 = TraversePathI(corner1, corner2, n2);
        Cell *path3 = TraversePathI(corner2, *c2, n3);
        n = n1 + n2 + n3 - 2;
        Cell *path = MergeArrays<Cell>(n1 - 1, path1, n2 - 1, path2, n3, path3);
        delete[] path1;
        delete[] path2;
        delete[] path3;
        return path;
    }

    return nullptr;
}

Cell* TraversePathI(Cell c1, Cell c2, int &n) {
    if (c1.x == c2.x) {
        bool flag = true;
        n = c2.y - c1.y + 1;
        if (c1.y > c2.y) {
            n = c1.y - c2.y + 1;
            flag = false;
        }

        Cell *path = new Cell[n];
        if (flag) {
            for (int y = c1.y; y <= c2.y; ++y) {
                path[y - c1.y] = Cell(c1.x, y);
            }
        } else {
            for (int y = c2.y; y <= c1.y; ++y) {
                path[c1.y - y] = Cell(c1.x, y);
            } 
        }

        return path;
    } else if (c1.y == c2.y) {
        bool flag = true;
        n = c2.x - c1.x + 1;
        if (c1.x > c2.x) {
            flag = false;
            n = c1.x - c2.x + 1;
        }
        
        // c1.x < x < c2.x
        Cell *path = new Cell[n];
        if (flag) {
            for (int x = c1.x; x <= c2.x; ++x) {
                path[x - c1.x] = Cell(x, c1.y);
            }
        } else { 
            for (int x = c2.x; x <= c1.x; ++x) {
                path[c1.x - x] = Cell(x, c1.y);
            }
        }
        return path;
    }
    
    return nullptr;
}

bool AreMatchingI(Board &board, const Cell &c1, const Cell &c2) {
    if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
        return false;
    }

    if (c1.x == c2.x) {
        int start = c1.y, end = c2.y;
        if (c1.y > c2.y) {
            start = c2.y;
            end = c1.y;
        }
        
        for (int i = start + 1; i <= end - 1; ++i) {
            if (!IsCellEmpty(board, c1.x, i)) {
                return false;
            }         
        }

        return true;
    } else if (c1.y == c2.y) {
        int start = c1.x, end = c2.x;
        if (c1.x > c2.x) {
            start = c2.x;
            end = c1.x;
        }
        
        for (int i = start + 1; i <= end - 1; ++i) {
            if (!IsCellEmpty(board, i, c1.y)) {
                return false;
            }         
        }

        return true;
    }

    return false;
}

bool AreMatchingL(Board &board, const Cell &c1, const Cell &c2, Cell &corner) {
    if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
        return false;
    }
    corner = Cell(c1.x, c2.y);
    if (AreMatchingI(board, c1, corner) && AreMatchingI(board, corner, c2)) {
        return true;
    }
    
    corner = Cell(c2.x, c1.y);
    if (AreMatchingI(board, c1, corner) && AreMatchingI(board, corner, c2)) {
        return true;
    }

    return false;
}

bool AreMatchingU(Board &board, Cell c1, Cell c2, Cell &corner1, Cell &corner2) {
    if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
        return false;
    }

    // Check U horizontally
    for (int i = 0; i < 2; ++i) {
        int x = c1.x;
        int y = c1.y;
        bool flag = false;
        if (i == 0 && c1.x < c2.x || i == 1 && c1.x > c2.x) {
            x = c2.x;
            y = c2.y;
            swap(c1, c2);
            flag = true;
        }
        
        if (i == 0)
            x++;
        else 
            x--;

        while (IsCellEmpty(board, x, y)) {
            if (AreMatchingL(board, Cell(x, y), c2, corner2)) {
                corner1 = Cell(x, y);
                if (flag) {
                    swap(corner1, corner2);
                }
                return true;     
            }

            if (i == 0) 
                x++;
            else 
                x--;   
        }

        if (flag) {
            swap(c1, c2);
        }
    }

    // Check U vertically
    for (int i = 0; i < 2; ++i) {
        int x = c1.x;
        int y = c1.y;
        bool flag = false;
        if (i == 0 && c1.y < c2.y || i == 1 && c1.y > c2.y) {
            x = c2.x;
            y = c2.y;
            swap(c1, c2);
            flag = true;
        }
        
        if (i == 0)
            y++;
        else 
            y--;

        while (IsCellEmpty(board, x, y)) {
            if (AreMatchingL(board, Cell(x, y), c2, corner2)) {
                corner1 = Cell(x, y);
                if (flag) {
                    swap(corner1, corner2);
                }
                return true;     
            }

            if (i == 0) 
                y++;
            else 
                y--;   
        }

        if (flag) {
            swap(c1, c2);
        }
    }

    return false;
} 

bool AreMatchingZ(Board &board, Cell c1, Cell c2, Cell &corner1, Cell &corner2) {
    if (!IsCellEmpty(board, c1.x, c1.y) && !IsCellEmpty(board, c2.x, c2.y) && board.cells[c1.x][c1.y] != board.cells[c2.x][c2.y]) {
        return false;
    }
    // Check x
    bool flag = false;
    if (c1.x > c2.x) {
        swap(c1, c2);
        flag = true;
    }

    Cell cur(c1.x + 1, c1.y);
    bool found = false;
    while (cur.x < c2.x && IsCellEmpty(board, cur.x, cur.y)) {
        if (AreMatchingL(board, cur, c2, corner2)) {
            corner1 = cur;
            found = true;
            break;
        }

        ++cur.x;
    }

    if (found) {
        if (flag) {
            swap(corner1, corner2);
        }
        return true;
    }

    if (flag) {
        swap(c1, c2);
    }

    // Check y
    flag = false;
    if (c1.y > c2.y) {
        swap(c1, c2);
        flag = true;
    }

    cur = Cell(c1.x, c1.y + 1);
    found = false;
    while (cur.y < c2.y && IsCellEmpty(board, cur.x, cur.y)) {
        if (AreMatchingL(board, cur, c2, corner2)) {
            corner1 = cur;
            found = true;
            break;
        }

        cur.y++;
    }

    if (found) {
        if (flag)
            swap(corner1, corner2);
        return true;
    }

    return false;
}