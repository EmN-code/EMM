#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

#define THREAD_STACKSIZE_MAIN THREAD_STACKSIZE_DEFAULT

char stack[THREAD_STACKSIZE_MAIN];

void *receiver(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        if (msg_receive(&msg) == 1) {
            printf("Received a message with type %d\n", msg.type);
        }
        xtimer_sleep(1);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, receiver, NULL, "receiver");
    if (pid > 0) {
        printf("Receiver thread started with PID %d\n", pid);
        msg_t msg;
        msg.type = 1;
        while (1) {
            msg_send(&msg, pid);
            xtimer_sleep(2);
        }
    }
    return 0;
}