#ifndef GAME_STRUCTS_H_
#define GAME_STRUCTS_H_

#include <string>

#include "linked_list.h"

using namespace std;

struct Cell {
    int x, y;  
    Cell(int x, int y): x(x), y(y) {}
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