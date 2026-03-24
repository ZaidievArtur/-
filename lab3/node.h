#pragma once
#include <pthread.h>

struct Node {
    int value;
    Node* next;
    pthread_mutex_t lock;

    Node(int val) : value(val), next(nullptr) {
        pthread_mutex_init(&lock, NULL);
    }
};
