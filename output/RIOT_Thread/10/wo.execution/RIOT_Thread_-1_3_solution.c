#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>
#include <time_units.h>
#include <timex.h>
#include <stdio.h>
#include <mbox.h>
#include <msg.h>
#include <thread.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>

#define THREAD_ONE_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define THREAD_TWO_PRIORITY   (THREAD_PRIORITY_MAIN - 2)
#define THREAD_ONE_STACKSIZE  THREAD_STACKSIZE_MAIN
#define THREAD_TWO_STACKSIZE  THREAD_STACKSIZE_MAIN

static char thread_one_stack[THREAD_ONE_STACKSIZE];
static char thread_two_stack[THREAD_TWO_STACKSIZE];

msg_t msg_queue[8];

void *thread_one(void *arg) {
    (void)arg;
    msg_init_queue(msg_queue, ARRAY_SIZE(msg_queue));
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Received message: %d\n", msg.content.value);
    }
    return NULL;
}

void *thread_two(void *arg) {
    (void)arg;
    msg_t msg;
    msg.type = 0x1234;
    msg.content.value = 42;
    while (1) {
        xtimer_sleep(1);
        msg_send(&msg, thread_getpid());
        printf("Sent message: %d\n", msg.content.value);
    }
    return NULL;
}

int main(void) {
    kernel_pid_t thread_one_pid = thread_create(thread_one_stack, sizeof(thread_one_stack), THREAD_ONE_PRIORITY, 0, thread_one, NULL, "thread_one");
    kernel_pid_t thread_two_pid = thread_create(thread_two_stack, sizeof(thread_two_stack), THREAD_TWO_PRIORITY, 0, thread_two, NULL, "thread_two");
    (void)thread_one_pid;
    (void)thread_two_pid;
    return 0;
}