#ifndef GAME_LINKED_LIST_H_
#define GAME_LINKED_LIST_H_

struct Node {
    Node *next = nullptr;
    // A pointer to a primitive or class, no array!
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

Node* ListGet(List &L, int i);

bool ListContains(List &L, void *data, bool (*pred)(void*, void*));

int ListLength(List &L);

// No ClearList, as it is left to the caller. We don't know how to delete variable "data" of Node.
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