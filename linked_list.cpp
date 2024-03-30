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

Node* ListGet(List &L, int i) {
    if (i < 0)
        return nullptr;
    
    Node *cur = L.head;
    for (int j = 1; j <= i && cur; ++j) {
        cur = cur->next;
    }
    return cur;
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

int ListLength(List &L) {
    int len = 0;
    Node *cur = L.head;
    while (cur) {
        ++len;
        cur = cur->next;
    }
    return len;
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