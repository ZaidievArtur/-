#include "coarse_set.h"

CoarseSet::CoarseSet() {
    head = nullptr;
    pthread_mutex_init(&global_lock, NULL);
}

bool CoarseSet::add(int item) {
    pthread_mutex_lock(&global_lock);

    Node* curr = head;
    while (curr) {
        if (curr->value == item) {
            pthread_mutex_unlock(&global_lock);
            return false;
        }
        curr = curr->next;
    }

    Node* node = new Node(item);
    node->next = head;
    head = node;

    pthread_mutex_unlock(&global_lock);
    return true;
}

bool CoarseSet::remove(int item) {
    pthread_mutex_lock(&global_lock);

    Node* curr = head;
    Node* prev = nullptr;

    while (curr) {
        if (curr->value == item) {
            if (prev) prev->next = curr->next;
            else head = curr->next;

            delete curr;
            pthread_mutex_unlock(&global_lock);
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    pthread_mutex_unlock(&global_lock);
    return false;
}

bool CoarseSet::contains(int item) {
    pthread_mutex_lock(&global_lock);

    Node* curr = head;
    while (curr) {
        if (curr->value == item) {
            pthread_mutex_unlock(&global_lock);
            return true;
        }
        curr = curr->next;
    }

    pthread_mutex_unlock(&global_lock);
    return false;
}

