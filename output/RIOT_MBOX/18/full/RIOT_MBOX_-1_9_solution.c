#include <stddef.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <atomic.h>
#include <stdbool.h>
#include <event/thread.h>
#include <stdio.h>
#include <esp_system.h>
#include <esp_wifi.h>
#include <mqueue.h>

#define STACK_SIZE 1024
#define MBOX_SIZE 8
#define MESSAGE_TYPE 1

static msg_t mbox_queue[MBOX_SIZE];
static mbox_t mbox;

void *sender_task(void *arg) {
    msg_t msg;
    int count = 0;

    while (1) {
        msg.type = MESSAGE_TYPE;
        msg.content.value = count++;
        printf("Sending message %d\n", msg.content.value);
        mbox_send(&mbox, &msg, 1);  // Use mbox_try_send if you don't want to block
        sleep(1);
    }
    return NULL;
}

void *receiver_task(void *arg) {
    msg_t msg;

    while (1) {
        mbox_receive(&mbox, &msg);  // This will block waiting for a message
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    mbox_init(&mbox, mbox_queue, MBOX_SIZE);
    pthread_t sender_thread, receiver_thread;

    // Create sender and receiver threads
    pthread_create(&sender_thread, NULL, sender_task, NULL);
    pthread_create(&receiver_thread, NULL, receiver_task, NULL);

    // Wait for threads to finish (they won't in this example)
    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);

    return 0;
}