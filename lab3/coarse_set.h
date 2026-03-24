#pragma once
#include "node.h"

class CoarseSet {
private:
    Node* head;
    pthread_mutex_t global_lock;

public:
    CoarseSet();
    bool add(int item);
    bool remove(int item);
    bool contains(int item);
};
