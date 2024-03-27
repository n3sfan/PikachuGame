#ifndef GAME_LINKED_LIST_H_
#define GAME_LINKED_LIST_H_

struct Node {
    Node *next = nullptr;
    // A pointer to a primitive or class, not array!
    void *data = nullptr;
};

struct List {
    Node *head = nullptr, *tail = nullptr;
    int size = 0;
};

struct Queue: List {
};

List& CreateList(Node *head);

Node* AddToBeginning(List &L, void *data);

Node* AddToEnd(List &L, void *data);

Node* RemoveAtPos(List &L, Node *pos, Node *prv);

bool ListContains(List &L, void *data, bool (*pred)(void*, void*));

/* Queue */
Queue& CreateQueue();

Node* Enqueue(Queue &Q, void *data);

Node* EnqueueTail(Queue &Q, void *data);

/**
 * Warning: Deallocate data
*/
void Dequeue(Queue &Q, void *data);

void* FrontQueue(Queue &Q);

void* TailQueue(Queue &Q);

void ClearQueue(Queue &Q);
#endif // GAME_LINKED_LIST_H_ 