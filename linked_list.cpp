#include "linked_list.h"

List& CreateList(Node *head) {
    List *L = new List;
    L->head = L->tail = head;
    return *L;
}

// TODO Begin and End: copy pointer data
Node* AddToBeginning(List &L, void *data) {
    Node *cur = new Node;
    cur->data = data;
    cur->next = nullptr;

    if (!L.head) {
        L.head = L.tail = cur;
    } else {
        cur->next = L.head;
        L.head = cur;
    }
    L.size++;
    return cur;
}

Node* AddToEnd(List &L, void *data) {
    Node *cur = new Node;
    cur->data = data;
    cur->next = nullptr;

    if (!L.head) {
        L.head = L.tail = cur;
    } else {
        L.tail->next = cur;
        L.tail = cur;
    }
    L.size++;
    return cur;
}

Node* RemoveAtPos(List &L, Node *pos, Node *prv) {
    if (!L.head || !pos) {
        return nullptr;
    }

    L.size--;
    if (!prv) { // Remove head
        Node *tmp = L.head;
        L.head = L.head->next;
        delete tmp;
        return L.head;
    } else {
        prv->next = pos->next;
        delete pos;
        return prv->next;
    }
}

bool ListContains(List &L, void *data, bool (*pred)(void*, void*)) {
    Node *cur = L.head;
    while (cur) {
        if (pred(cur->data, data)) {
            return true;
        }

        cur = cur->next;
    }

    return false;
}

/* Queue */
//Queue& CreateQueue() {
    //Queue *Q = new Queue;
    //return *Q;
//}

//Node* Enqueue(Queue &Q, void *data) {
    //AddToEnd(Q, data);
//}

//Node* EnqueueTail(Queue &Q, void *data) {
    //AddToBeginning(Q, data);
//}

//void* FrontQueue(Queue &Q) {
    //return Q.head->data;
//}

//void* TailQueue(Queue &Q) {
    //return Q.tail->data;
//}

//void Dequeue(Queue &Q) {
    //RemoveAtPos(Q, Q.head, nullptr);
//}

/* Arrays */