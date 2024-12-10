
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// Define the message structure
typedef struct {
    int sender_pid;
    int type;
    union {
        void *ptr;
        int value;
    } content;
} msg_t;

// Define the thread function for the sender
void *sender_thread(void *arg) {
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;

    // Simulate sending a message to the receiver thread
    printf("Sender: Sending message with value %d\n", msg.content.value);
    // In a real RIOT application, you would use msg_send_receive here
    // For this example, we'll just print the message
    printf("Sender: Message sent\n");

    return NULL;
}

// Define the thread function for the receiver
void *receiver_thread(void *arg) {
    msg_t msg;

    // Simulate receiving a message from the sender thread
    printf("Receiver: Waiting for message...\n");
    // In a real RIOT application, you would use msg_receive here
    // For this example, we'll just print the received message
    printf("Receiver: Message received with value %d\n", msg.content.value);

    return NULL;
}

int main() {
    pthread_t sender, receiver;

    // Create the sender thread
    if (pthread_create(&sender, NULL, sender_thread, NULL) != 0) {
        perror("Failed to create sender thread");
        return 1;
    }

    // Create the receiver thread
    if (pthread_create(&receiver, NULL, receiver_thread, NULL) != 0) {
        perror("Failed to create receiver thread");
        return 1;
    }

    // Wait for the sender thread to finish
    if (pthread_join(sender, NULL) != 0) {
        perror("Failed to join sender thread");
        return 1;
    }

    // Wait for the receiver thread to finish
    if (pthread_join(receiver, NULL) != 0) {
        perror("Failed to join receiver thread");
        return 1;
    }

    printf("Main: All threads have finished\n");

    return 0;
}
