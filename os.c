#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BUFFER_SIZE 5
int buffer[BUFFER_SIZE];
int in = 0;
// points to the next empty position
int out = 0;
// points to the next full position
sem_t empty;
// semaphore counting empty slots
sem_t full;
// semaphore counting full slots
pthread_mutex_t mutex;
// mutex for buffer access

void* producer(void* arg)
{
    int item;
    for (int i = 0; i < 10; i++)
	{
        item = i + 1;
		// Produce items numbered 1 to 10
        sem_wait(&empty);
		// Wait for empty slot
        pthread_mutex_lock(&mutex);
		// Enter critical section
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produced: %d\n", item);
        pthread_mutex_unlock(&mutex);
		// Leave critical section
        sem_post(&full);
		// Signal that new item is available
        sleep(1);
		//simulate production time
    }
    pthread_exit(NULL);
}
void* consumer(void* arg)
{
    int item;
    for (int i = 0; i < 10; i++)
	{
        sem_wait(&full);
		// Wait for full slot
        pthread_mutex_lock(&mutex);
		// Enter critical section
        item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumed: %d\n", item);
        pthread_mutex_unlock(&mutex);
		// Leave critical section
        sem_post(&empty);
		// Signal that slot is free
        sleep(2);
		// simulate consumption time
    }
    pthread_exit(NULL);
}
int main()
{
    pthread_t prod_thread, cons_thread;
    // Initialize semaphore values
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);
    // Wait for threads to finish
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    // Cleanup semaphores and mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}