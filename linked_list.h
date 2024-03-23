#ifndef GAME_LINKED_LIST_H_
#define GAME_LINKED_LIST_H_

struct Node {
    Node *next = nullptr;
    void *data = nullptr;
};

struct List {
    Node *head = nullptr, *tail = nullptr;
    int size = 0;
};

List& CreateList(Node *head);

Node* AddToBeginning(List &L, void *data);

Node* AddToEnd(List &L, void *data);

Node* RemoveAtPos(List &L, Node *pos, Node *prv);

#endif // GAME_LINKED_LIST_H_ 