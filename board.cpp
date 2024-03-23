#include "board.h"
#include "linked_list.h"

const int search_dx[] = {-1, 0, 1, 0};
const int search_dy[] = {0, 1, 0, -1};

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
    for (int i = 1; i < m - 1; ++i) {
        for (int j = 1; j < n - 1; ++j) {
            board->cells[i][j] = 'A' + rand()%26; 
        }
    }

    return *board;
}

void DeleteBoard(Board &board) { 
    for (int i = 0; i < board.m; ++i) {
        delete[] board.cells[i];
    }
    delete[] board.cells;
    delete &board;
}

bool IsCellEmpty(Board &board, int x, int y) {
    return IsInside(board, x, y) && (board.cells[x][y] == 0 || board.cells[x][y] == ' ');
}

bool IsInside(Board &board, int x, int y) {
    return 1 <= x && x < board.m - 1 && 1 <= y && y < board.n - 1;
}

void GoToBoardCell(Board &board, int x, int y) {
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
void RemoveChosenCell(Board &board, int x, int y) {
    Node *prv = nullptr;
    Node *cur = board.chosen_cells.head;

    while (cur) {
        Cell* cell = (Cell*) cur->data;
        
        if (cell->x == x && cell->y == y) {
            RemoveAtPos(board.chosen_cells, cur, prv);
            break;
        }

        prv = cur;
        cur = cur->next;
    }
}

bool CheckMatching(Board &board) {
    if (board.chosen_cells.size < 2) 
        return false;

    return false;
}

// bool TraverseTo(Board &board, int x, int y, int dest_x, int dest_y, List &path, int turn_type = -1) {
//     if (x == dest_x && y == dest_y) {
//         return true;
//     }

//     // for (int i = 0; i < sizeof(search_dx) / sizeof(int); ++i) {
//     //     int next_x = x + search_dx[i];
//     //     int next_y = y + search_dy[i];

//     //     if (IsCellEmpty(board, next_x, next_y)) {
//     //         AddToEnd(path, board.cells[next_x][next_y]);
//     //         TraverseTo(board, next_x next_y, dest_x, next_y, )
//     //     }
//     // } 
// }