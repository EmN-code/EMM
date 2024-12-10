
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_MESSAGES 10
#define STACK_SIZE 1024

typedef struct {
    int priority;
    char message[50];
} thread_message_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *sender_thread(void *arg) {
    thread_message_t *messages = (thread_message_t *)arg;
    for (int i = 0; i < NUM_MESSAGES; i++) {
        pthread_mutex_lock(&mutex);
        printf("Sending message: %s with priority %d\n", messages[i].message, messages[i].priority);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100000); // Simulate delay
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    thread_message_t *messages = (thread_message_t *)arg;
    for (int i = 0; i < NUM_MESSAGES; i++) {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&cond, &mutex);
        printf("Received message: %s with priority %d\n", messages[i].message, messages[i].priority);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t sender, receiver;
    thread_message_t messages[NUM_MESSAGES];

    for (int i = 0; i < NUM_MESSAGES; i++) {
        messages[i].priority = i % 3; // Different priorities
        snprintf(messages[i].message, sizeof(messages[i].message), "Message %d", i);
    }

    pthread_create(&sender, NULL, sender_thread, messages);
    pthread_create(&receiver, NULL, receiver_thread, messages);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}
