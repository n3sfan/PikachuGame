#include "structs.h"

// template <typename U>
// bool operator==(const Cell<U> &a, const Cell<U> &o) {
//     return a.x == o.x && a.y == o.y;
// }

bool operator==(const Cell &a, const Cell &o) {
    return a.x == o.x && a.y == o.y;
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