#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <msg.h>
#include <time_units.h>
#include <thread.h>
#include <xtimer.h>

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

msg_t msg_queue[8];  // Message queue must be power of two

char sender_stack[STACK_SIZE];
char receiver_stack[STACK_SIZE];

void *sender_thread(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 1;
    msg.content.value = 42;  // Example value

    while (1) {
        msg_send(&msg, thread_getpid() + 1);  // Sending message to receiver thread
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_thread(void *arg) {
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %d and value: %d\n", msg.type, msg.content.value);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);

    thread_create(receiver_stack, STACK_SIZE, THREAD_PRIORITY_RECEIVER,
                  THREAD_CREATE_STACKTEST, receiver_thread, NULL, "receiver");
    thread_create(sender_stack, STACK_SIZE, THREAD_PRIORITY_SENDER,
                  THREAD_CREATE_STACKTEST, sender_thread, NULL, "sender");

    return 0;
}