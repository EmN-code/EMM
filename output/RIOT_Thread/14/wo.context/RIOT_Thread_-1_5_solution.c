#include <xtimer.h>
#include <stdio.h>
#include <thread.h>
#include "msg.h"

#define THREAD_PRIORITY_SENDER (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_RECEIVER (THREAD_PRIORITY_MAIN - 2)
#define THREAD_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)

static char sender_stack[THREAD_STACKSIZE];
static char receiver_stack[THREAD_STACKSIZE];

static kernel_pid_t receiver_pid;

void *sender_function(void *arg) {
    (void)arg;
    msg_t msg;
    msg.content.value = 42; // Example message value
    while (1) {
        printf("Sender: Sending message with value %ld\n", msg.content.value);
        msg_send(&msg, receiver_pid);
        xtimer_sleep(2);
    }
    return NULL;
}

void *receiver_function(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Receiver: Received message with value %ld\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    printf("Starting threading example on RIOT\n");
    receiver_pid = thread_create(receiver_stack, sizeof(receiver_stack),
                                 THREAD_PRIORITY_RECEIVER, 0,
                                 receiver_function, NULL, "receiver_thread");
    thread_create(sender_stack, sizeof(sender_stack),
                  THREAD_PRIORITY_SENDER, 0,
                  sender_function, NULL, "sender_thread");
    
    return 0;
}