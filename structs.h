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
    char letter = ' ';
    Cell(int x = 0, int y = 0): x(x), y(y) {}

    friend bool operator==(const Cell &a, const Cell &o);
};

struct Player {
    string name;
    int score;
};

struct Board {
    int m, n;
    int cur_x = 0, cur_y = 0;

    Board(int m, int n);

    virtual char GetLetter(int x, int y) = 0;
    virtual void SetLetter(int x, int y, char letter) = 0;
    virtual void RemoveCell(int x, int y) = 0;
    /**
     * Including 2 empty border rows.
    */
    virtual int GetGameRows() = 0;
    /**
     * Including 2 empty border rows.
    */
    virtual int GetGameRowSize(int x) = 0;
    
    /** 
     * Elements are Cells.
     * Cell 0: (x1, y1). Cell 1 : (x2, y2)  
    */
    List chosen_cells; 
};

struct BoardInt: Board {
    char **cells;  

    BoardInt(int m, int n);

    char GetLetter(int x, int y) override;
    void SetLetter(int x, int y, char letter) override;
    void RemoveCell(int x, int y) override;
    int GetGameRows() override;
    int GetGameRowSize(int x) override;

    ~BoardInt();
};

struct BoardLL: Board {
    /**
     * m, n is updated.
    */
    /**
     * Can only be a pointer here, reference cannot be used.
     * Not reference or pointer is not a choice either, as it copy the List created.
    */
    List* cells;
    int game_rows;

    BoardLL(int m, int n);

    char GetLetter(int x, int y) override;
    void SetLetter(int x, int y, char letter) override;
    void RemoveCell(int x, int y) override;
    int GetGameRows() override;
    int GetGameRowSize(int x) override;


    ~BoardLL();
};

struct Account {
    string username;
    string password;
};

// struct GameStage {
//     Account user;
//     Board board;
// };
#endif // GAME_STRUCTS_H_