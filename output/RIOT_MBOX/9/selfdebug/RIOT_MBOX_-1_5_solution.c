#include <stddef.h>  // Required for standard definitions
#include <stdio.h>    // Required for printf
#include <stdnoreturn.h>  // Required for noreturn
#include <stdlib.h>   // For exit function
#include <string.h>   // For memset function
#include <sys/types.h>  // For standard system types

#include "kernel_structs.h"  // Definition for kernel PID
#include "xtimer.h"  // Required for using timers
#include "msg.h"     // Message functionality

#define MAIN_QUEUE_SIZE     (8)     // Size of the queue for main thread

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];  // Message queue
static evtimer_t evtimer;  // Event timer

void *thread_send(void *arg) {
    msg_t msg;
    msg.content.value = 0;
    while (1) {
        for (int i = 0; i < 5; i++) {
            xtimer_sleep(2);  // Sleep for 2 seconds
            msg.content.value++;
            mbox_put((mbox_t *)arg, &msg);  // Put a message in the mailbox
            printf("Message sent: %d\n", msg.content.value);
        }
    }
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mbox_t mbox;
    mbox_init(&mbox, _main_msg_queue, MAIN_QUEUE_SIZE);  

    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  thread_send, &mbox, "send");

    msg_t msg;
    while (1) {
        mbox_get(&mbox, &msg);  // Receive a message from the mailbox
        printf("Message received: %d\n", msg.content.value);
    }

    return 0;
}