#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"

#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)
#define THREAD_PRIORITY_LOW      (THREAD_PRIORITY_MAIN + 1)

char stack_low[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[8];

void *thread_low(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    while (1) {
        printf("Thread Low: Waiting for a message...\n");
        msg_receive(&msg);
        printf("Thread Low: Received message of type: %04x\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    kernel_pid_t low_thread_pid = thread_create(stack_low, sizeof(stack_low), THREAD_PRIORITY_LOW, 0, thread_low, NULL, "thread_low");
    msg_t msg;
    msg.type = 0x1234;
    while (1) {
        printf("Main Thread: Sending message...\n");
        msg_send(&msg, low_thread_pid);
        xtimer_sleep(2);
    }
    return 0;
}