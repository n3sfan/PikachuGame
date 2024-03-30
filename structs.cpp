#include "structs.h"

// template <typename U>
// bool operator==(const Cell<U> &a, const Cell<U> &o) {
//     return a.x == o.x && a.y == o.y;
// }

bool operator==(const Cell &a, const Cell &o) {
    return a.x == o.x && a.y == o.y;
}

/** Board **/
Board::Board(int m, int n): m(m), n(n) {
}

/* BoardInt */
BoardInt::BoardInt(int m, int n): Board(m, n) {
    cells = new char*[m];
    for (int i = 0; i < m; ++i) {
        cells[i] = new char[n] {};
    }
}

char BoardInt::GetLetter(int x, int y) {
    return cells[x][y];
}

void BoardInt::SetLetter(int x, int y, char c) {
    cells[x][y] = c;
}  

void BoardInt::RemoveCell(int x, int y) {
    cells[x][y] = ' ';
}

int BoardInt::GetGameRows() {
    return m;
}

int BoardInt::GetGameRowSize(int x) {
    return n;
}

BoardInt::~BoardInt() {
    for (int i = 0; i < m; ++i) {
        delete[] cells[i];
    }
    delete[] cells;
}

/* BoardLL */
BoardLL::BoardLL(int m, int n): Board(m, n) {
    cells = &CreateList(nullptr);
    for (int i = 0; i < m; ++i) {
        List &row = CreateList(nullptr);
        for (int j = 0; j < n; ++j) {
            AddToEnd(row, new char(' '));
        }        
        AddToEnd(*cells, &row);
    }
    game_rows = m;
}

char BoardLL::GetLetter(int x, int y) {
    Node *row = ListGet(*cells, x);
    if (!row)
        return 0;
    Node *col = ListGet(*(List*) row->data, y);
    if (!col)
        return 0;
    return *((char*) col->data);
}

void BoardLL::SetLetter(int x, int y, char c) { 
    Node *row = ListGet(*cells, x);
    if (!row)
        return;
    Node *col = ListGet(*(List*) row->data, y);
    if (!col)
        return;
    *(char*) col->data = c;
}

void BoardLL::RemoveCell(int x, int y) {
    Node *prv_row, *row;
    if (x == 0) {
        prv_row = nullptr;
        row = cells->head;
    } else {
        prv_row = ListGet(*cells, x - 1);
        row = prv_row->next;
    }
    if (!row)
        return;
    
    Node *prv_col, *col;
    List *row_L = (List*) row->data;
    if (y == 0) {
        prv_col = nullptr;
        col = row_L->head;
    } else {
        prv_col = ListGet(*row_L, y - 1);
        col = prv_col->next;
    }
    if (!col)
        return;

    delete (char*) col->data;
    RemoveAtPos(*row_L, col, prv_col);
    AddToEnd(*row_L, new char(' '));

    if (GetLetter(x, 1) == ' ') {
        void *old_list = row->data;
        RemoveAtPos(*cells, row, prv_row);
        AddToEnd(*cells, old_list);
        --game_rows;
    }
}

int BoardLL::GetGameRows() {
    return game_rows;
}

int BoardLL::GetGameRowSize(int x) {
    // Always >= 2 elements
    List *row_l = (List*) ListGet(*cells, x)->data;
    Node *cur = row_l->head->next;
    int len = 2;
    while (cur && *(char*) cur->data != ' ') {
        ++len;
        cur = cur->next;
    }
    return len;
}

BoardLL::~BoardLL() {
    Node *row = cells->head;
    while (row) {
        Node *tmp1 = row->next;
        Node *col = ((List*) row->data)->head;
        while (col) {
            Node *tmp2 = col->next;
            delete (char*) col->data; // of type char* (single char)
            delete col;
            col = tmp2;
        }       
        
        delete (List*) row->data; // of type List*
        delete row;
        row = tmp1;
    }
}
// template<typename T>
// struct TStruct {
//     T a;
// };

// typedef TStruct<int> TInt;

// template<typename T>
// void test(TStruct<T> it){
    
// }

// void test2() {
//     test(TInt { 1});
// }