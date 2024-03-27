#ifndef GAME_STRUCTS_H_
#define GAME_STRUCTS_H_

#include <string>

#include "linked_list.h"
#include <iterator>

using namespace std;

// template <typename T>
// struct Cell {
//     T x, y;  
//     Cell(T x = 0, T y = 0): x(x), y(y) {}

//     template <typename U>
//     friend bool operator==(const Cell<U> &a, const Cell<U> &o);
// };

// struct CellInt: Cell<int> {
// };

// struct CellNode: Cell<Node*> {
// };

struct Cell {
    int x, y;
    Cell(int x = 0, int y = 0): x(x), y(y) {}

    friend bool operator==(const Cell &a, const Cell &o);
};

struct Board {
    char **cells;  
    int m, n;
    int cur_x = 0, cur_y = 0;
    
    /** 
     * Elements are Cells.
     * Cell 0: (x1, y1). Cell 1 : (x2, y2)  
    */
    List chosen_cells; 
};

struct Account {
    string username;
    string password;
};

struct GameStage {
    Account user;
    Board board;
};
#endif // GAME_STRUCTS_H_