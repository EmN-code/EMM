#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "msg.h"
#include "kernel_types.h"

#define INTERVAL (1U * US_PER_SEC)
#define QUEUE_SIZE (8)

static msg_t msg_queue[QUEUE_SIZE];

static void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    /* initialize a message queue */
    msg_init_queue(msg_queue, QUEUE_SIZE);
    
    char thread_stack[KERNEL_PID_LAST+1];
    /* create a thread for message processing */
    kernel_pid_t thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                                            THREAD_PRIORITY_MAIN, 0,
                                            thread_handler, NULL, "receiver");

    if (thread_pid <= KERNEL_PID_LAST) {
        printf("Failed to create thread\n");
        return 1;
    }

    /* send messages at intervals */
    msg_t msg;
    msg.type = 0x1234;

    while (1) {
        msg_send(&msg, thread_pid);
        printf("Sent message with type: %u\n", msg.type);
        xtimer_sleep(INTERVAL);
    }

    return 0;
}