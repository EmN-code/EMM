#include <xtimer.h>
#include <thread.h>
#include <stdio.h>
#include <msg.h>

#define MAIN_QUEUE_SIZE  (8)
#define STACKSIZE        THREAD_STACKSIZE_MAIN

static kernel_pid_t thread_pid;
static char stack[STACKSIZE];
static msg_t msg_queue[MAIN_QUEUE_SIZE];

void *thread_handler(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message with type: %u\n", msg.type);
    }
    return NULL;
}

int main(void) {
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);
    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                               thread_handler, NULL, "receiver");

    msg_t msg;
    msg.type = 0x01;
    while (1) {
        msg_send(&msg, thread_pid);
        xtimer_sleep(1);
    }
    return 0;
}