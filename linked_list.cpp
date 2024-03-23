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
    return cur;
}

Node* RemoveAtPos(List &L, Node *pos, Node *prv) {
    if (!L.head || !pos) {
        return nullptr;
    }

    if (!prv) { // Remove head
        Node *tmp = L.head;
        L.head = L.head->next;
        // TODO Deallocate continuous memory chunks
        delete tmp;
        return L.head;
    } else {
        prv->next = pos->next;
        // TODO Same
        delete pos;
        return prv->next;
    }
}