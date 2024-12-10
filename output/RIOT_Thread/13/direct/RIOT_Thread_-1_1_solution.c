#include "msg.h"
#include "thread.h"
#include <stdio.h>

#define THREAD_STACKSIZE_MAIN 1024
#define THREAD_PRIORITY_MAIN 5
#define THREAD_PRIORITY_SENDER 4
#define THREAD_PRIORITY_RECEIVER 3
#define MSG_QUEUE_SIZE 8

static char sender_stack[THREAD_STACKSIZE_MAIN];
static char receiver_stack[THREAD_STACKSIZE_MAIN];

void *sender(void *arg);
void *receiver(void *arg);

int main(void) {
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);

    thread_create(receiver_stack, sizeof(receiver_stack), THREAD_PRIORITY_RECEIVER, THREAD_CREATE_STACKTEST,
                  receiver, NULL, "receiver");
    
    thread_create(sender_stack, sizeof(sender_stack), THREAD_PRIORITY_SENDER, THREAD_CREATE_STACKTEST,
                  sender, NULL, "sender");

    while (1) {
        // Main thread loop or other operational logic
    }
    return 0;
}

void *sender(void *arg) {
    (void)arg;
    msg_t msg;
    int message_number = 0;

    while (1) {
        msg.content.value = message_number++;  // increment message content
        printf("Sender: Sending message %d\n", msg.content.value);
        msg_send(&msg, thread_getpid());  // send the message to receiver thread
        thread_yield();  // yield control to allow receiver to process message
    }
}

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        if (msg_receive(&msg)) {
            printf("Receiver: Received message %d\n", msg.content.value);
            // process the message
        }
        thread_yield();  // yield control back to sender
    }
}

msg_t msg_queue[MSG_QUEUE_SIZE];