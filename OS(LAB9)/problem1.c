#include <stdio.h>
#include <stdlib.h>  // Required for rand()
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 100
#define PAIR_SIZE 2

int buffer[BUFFER_SIZE];
int in = 0, out = 0; // Buffer pointers

sem_t empty, full, mutex;

void* producer(void* arg) {
    int P1 = rand() % 100, P2 = rand() % 100;

    sem_wait(&empty);
    sem_wait(&empty); // Reserve 2 slots

    sem_wait(&mutex); // Lock buffer
    buffer[in] = P1;
    buffer[(in + 1) % BUFFER_SIZE] = P2;
    in = (in + PAIR_SIZE) % BUFFER_SIZE;
    printf("Produced: (%d, %d)\n", P1, P2);
    sem_post(&mutex); // Unlock buffer

    sem_post(&full);
    sem_post(&full); // Signal that 2 slots are filled

    return NULL;
}

void* consumer(void* arg) {
    sem_wait(&full);
    sem_wait(&full); // Ensure at least 2 items

    sem_wait(&mutex); // Lock buffer
    int P1 = buffer[out];
    int P2 = buffer[(out + 1) % BUFFER_SIZE];
    out = (out + PAIR_SIZE) % BUFFER_SIZE;
    printf("Consumed: (%d, %d)\n", P1, P2);
    sem_post(&mutex); // Unlock buffer

    sem_post(&empty);
    sem_post(&empty); // Free up 2 slots

    return NULL;
}

int main() {
    pthread_t prod, cons;
    sem_init(&empty, 0, BUFFER_SIZE / PAIR_SIZE); // 50 pairs available initially
    sem_init(&full, 0, 0); // No full pairs initially
    sem_init(&mutex, 0, 1); // Mutex initialized to 1

    pthread_create(&prod, NULL, producer, NULL);
    pthread_join(prod, NULL);

    pthread_create(&cons, NULL, consumer, NULL);
    pthread_join(cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
