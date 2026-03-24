#include "fine_set.h"

FineSet::FineSet() {
    head = new Node(-1); // sentinel
}

bool FineSet::add(int item) {
    pthread_mutex_lock(&head->lock);

    Node* curr = head->next;
    while (curr) {
        if (curr->value == item) {
            pthread_mutex_unlock(&head->lock);
            return false;
        }
        curr = curr->next;
    }

    Node* node = new Node(item);
    node->next = head->next;
    head->next = node;

    pthread_mutex_unlock(&head->lock);
    return true;
}

bool FineSet::remove(int item) {
    Node* pred = head;
    pthread_mutex_lock(&pred->lock);

    Node* curr = pred->next;
    if (curr) pthread_mutex_lock(&curr->lock);

    while (curr) {
        if (curr->value == item) {
            pred->next = curr->next;

            pthread_mutex_unlock(&curr->lock);
            pthread_mutex_unlock(&pred->lock);

            delete curr;
            return true;
        }

        pthread_mutex_unlock(&pred->lock);
        pred = curr;
        curr = curr->next;
        if (curr) pthread_mutex_lock(&curr->lock);
    }

    pthread_mutex_unlock(&pred->lock);
    return false;
}

bool FineSet::contains(int item) {
    Node* curr = head->next;

    while (curr) {
        pthread_mutex_lock(&curr->lock);

        if (curr->value == item) {
            pthread_mutex_unlock(&curr->lock);
            return true;
        }

        Node* temp = curr;
        curr = curr->next;
        pthread_mutex_unlock(&temp->lock);
    }

    return false;
}
