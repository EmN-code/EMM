#include "string.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "stdio.h"

#define SLEEP_INTERVAL (1000000U) // Sleep interval in microseconds

mbox_t my_mbox;
msg_t my_msg_queue[10]; // Define the message queue

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    int counter = 0;
    while (1) {
        // Prepare message
        msg.content.value = counter++;

        // Put message into mailbox
        if (_mbox_get(&my_mbox, &msg, 0)) {
            printf("Sent message with value: %d\n", msg.content.value);
        }

        // Sleep for a defined interval
        usleep(SLEEP_INTERVAL);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        // Try to get message from the mailbox
        if (_mbox_get(&my_mbox, &msg, 0) == 1)  {
            printf("Received message with value: %d\n", msg.content.value);
        }
    }
    return NULL;
}

int main(void) {
    // Initialize the mailbox
    mbox_init(&my_mbox, my_msg_queue, 10);

    // Create sender and receiver threads
    pthread_t sender, receiver;
    pthread_create(&sender, NULL, sender_thread, NULL);
    pthread_create(&receiver, NULL, receiver_thread, NULL);

    // Wait for threads to finish (they won't in this example)
    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}