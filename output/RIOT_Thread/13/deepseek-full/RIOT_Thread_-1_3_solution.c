
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_MESSAGES 10
#define STACK_SIZE 1024

typedef struct {
    int priority;
    uint32_t value;
} message_t;

pthread_t sender_thread, receiver_thread;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
message_t messages[NUM_MESSAGES];
int message_index = 0;

void *sender(void *arg) {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        pthread_mutex_lock(&mutex);
        messages[message_index].priority = i % 3; // Different priorities
        messages[message_index].value = i + 1;
        printf("Sender: Sending message with priority %d and value %lu\n", messages[message_index].priority, (unsigned long)messages[message_index].value);
        message_index++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100000); // Simulate delay
    }
    return NULL;
}

void *receiver(void *arg) {
    for (int i = 0; i < NUM_MESSAGES; i++) {
        pthread_mutex_lock(&mutex);
        while (message_index == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        printf("Receiver: Received message with priority %d and value %lu\n", messages[i].priority, (unsigned long)messages[i].value);
        message_index--;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_create(&sender_thread, NULL, sender, NULL);
    pthread_create(&receiver_thread, NULL, receiver, NULL);

    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
