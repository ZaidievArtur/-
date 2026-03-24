#include "tests.h"
#include "coarse_set.h"
#include "fine_set.h"
#include <pthread.h>
#include <iostream>
#include <chrono>

const int N = 10000;
const int THREADS = 4;

CoarseSet coarseSet;
FineSet fineSet;

void* writer_coarse(void*) {
    for (int i = 0; i < N; i++) {
        coarseSet.add(i);
    }
    return NULL;
}

void* writer_fine(void*) {
    for (int i = 0; i < N; i++) {
        fineSet.add(i);
    }
    return NULL;
}

void test_coarse() {
    pthread_t threads[THREADS];

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, writer_coarse, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Coarse time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

void test_fine() {
    pthread_t threads[THREADS];

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < THREADS; i++)
        pthread_create(&threads[i], NULL, writer_fine, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Fine time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
              << " ms\n";
}

void run_all_tests() {
    std::cout << "Running tests...\n";
    test_coarse();
    test_fine();
}
