#pragma once
#include "node.h"

class FineSet {
private:
    Node* head;

public:
    FineSet();
    bool add(int item);
    bool remove(int item);
    bool contains(int item);
};
