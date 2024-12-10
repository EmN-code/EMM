#include <xtimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>

#define MSG_COUNT 16 // Define number of messages
#define INTERVAL_US 1000000 // Send event every second

// Structure for message
typedef struct {
    int id;
    char data[128];
} message_t;

pthread_mutex_t lock;
message_t mbox[MSG_COUNT];
int mbox_head = 0; // Head index of the mailbox
int mbox_tail = 0; // Tail index of the mailbox

void *sender_thread_func(void *arg) {
    int id = 0;
    while (1) {
        usleep(INTERVAL_US); // Wait for the defined interval

        pthread_mutex_lock(&lock);
        if ((mbox_head + 1) % MSG_COUNT != mbox_tail) { // Check if mailbox is not full
            mbox[mbox_head].id = id++;
            snprintf(mbox[mbox_head].data, sizeof(mbox[mbox_head].data), "Event #%d", mbox[mbox_head].id);
            printf("Sending: %s\n", mbox[mbox_head].data);
            mbox_head = (mbox_head + 1) % MSG_COUNT; // Move to next position
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void *receiver_thread_func(void *arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        if (mbox_head != mbox_tail) { // If there is a message in the mailbox
            printf("Received: %s\n", mbox[mbox_tail].data);
            mbox_tail = (mbox_tail + 1) % MSG_COUNT; // Move to next message
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t sender_thread, receiver_thread;
    int res;

    pthread_mutex_init(&lock, NULL);
    
    res = pthread_create(&sender_thread, NULL, sender_thread_func, NULL);
    if (res) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&receiver_thread, NULL, receiver_thread_func, NULL);
    if (res) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}