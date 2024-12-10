#include <timex.h>
#include <mbox.h>
#include <thread.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE     (8)
#define THREAD_STACKSIZE    THREAD_STACKSIZE_MAIN

static char thread_stack[THREAD_STACKSIZE];
static msg_t msg_queue[MAIN_QUEUE_SIZE];
static kernel_pid_t thread_pid;
static msg_t msg;

void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        msg_receive(&msg);
        puts("Received a message!");
    }
    return NULL;
}

int main(void) {
    mbox_t mbox;
    mbox_init(&mbox, msg_queue, MAIN_QUEUE_SIZE);
    thread_pid = thread_create(thread_stack, THREAD_STACKSIZE,
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               thread_handler, NULL, "thread");

    while (1) {
        msg_send(&msg, thread_pid);
        xtimer_sleep(5);
    }
    return 0;
}