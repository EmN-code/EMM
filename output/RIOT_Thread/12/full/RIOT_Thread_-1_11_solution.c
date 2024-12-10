// Required RIOT headers for the functionality
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <net/gcoap.h>
#include <net/emcute.h>
#include <net/sock/udp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <thread.h>
#include <unistd.h>
#include <xtimer.h>

#define MSG_QUEUE_SIZE 8

// Message structures
static msg_t msg_queue[MSG_QUEUE_SIZE];

// Thread stacks
static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

// Sender thread function
void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;
    printf("Sender: Sending message with value %" PRIu32 "\n", msg.content.value);
    while (1) {
        if (msg_send(&msg, thread_getpid()) <= 0) {
            printf("Sender: Failed to send message. Retrying...\n");
        } else {
            printf("Sender: Message sent successfully.\n");
            break;
        }
        xtimer_sleep(1);
    }
    return NULL;
}

// Receiver thread function
void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) <= 0) {
            printf("Receiver: Failed to receive message.\n");
        } else {
            printf("Receiver: Received message with value %" PRIu32 "\n", msg.content.value);
            break;
        }
    }
    return NULL;
}

int main(void) {
    printf("Main: Initializing message queue...\n");
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    // Create sender and receiver threads with different priorities
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, sender, NULL, "sender");
    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_MAIN - 2,
                  THREAD_CREATE_STACKTEST, receiver, NULL, "receiver");

    return 0;
}