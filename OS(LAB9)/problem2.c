#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Declare semaphores
sem_t a, b, c; 

// Process 1: Prints "H" and "E"
void* process1(void* arg) {
    sem_wait(&a);  // Wait for permission to start
    printf("H");
    printf("E");
    sem_post(&b);  // Signal Process 2 to print "L"
    return NULL;
}

// Process 2: Prints "L"
void* process2(void* arg) {
    sem_wait(&b);  // Wait until Process 1 prints "HE"
    printf("L");
    sem_post(&c);  // Signal Process 3 to print "LO"
    return NULL;
}

// Process 3: Prints "LO"
void* process3(void* arg) {
    sem_wait(&c);  // Wait until Process 2 prints "L"
    printf("L");
    printf("O");
    return NULL;
}

int main() {
    pthread_t p1, p2, p3;

    // Initialize semaphores
    sem_init(&a, 0, 1);  // Start with Process 1 enabled
    sem_init(&b, 0, 0);  // Process 2 waits for Process 1
    sem_init(&c, 0, 0);  // Process 3 waits for Process 2

    // Create threads
    pthread_create(&p1, NULL, process1, NULL);
    pthread_create(&p2, NULL, process2, NULL);
    pthread_create(&p3, NULL, process3, NULL);

    // Wait for threads to complete
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);

    // Destroy semaphores
    sem_destroy(&a);
    sem_destroy(&b);
    sem_destroy(&c);

    printf("\n"); // Ensure proper output formatting
    return 0;
}
