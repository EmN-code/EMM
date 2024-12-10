#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "stdatomic.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdint.h"
#include "msg.h"
#include "mbox.h"
#include "kernel_defines.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_MAIN)

char thread_stack1[THREAD_STACKSIZE_MAIN];
char thread_stack2[THREAD_STACKSIZE_MAIN];

static kernel_pid_t thread1_pid;
static kernel_pid_t thread2_pid;

void* thread1_func(void *arg) {
    (void)arg;
    msg_t msg;
    for (uint32_t i = 0; i < 10; ++i) {
        msg.content.value = i;
        printf("Thread 1 sending message: %" PRIu32 "\n", i);
        msg_send(&msg, thread2_pid);
        xtimer_sleep(1);
    }
    return NULL;
}

void* thread2_func(void *arg) {
    (void)arg;
    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Thread 2 received message: %" PRIu32 "\n", msg.content.value);
    }
    return NULL;
}

int main(void)
{
    msg_init_queue(msg_queue, THREAD_PRIORITY_MAIN);
    thread2_pid = thread_create(thread_stack2, sizeof(thread_stack2), THREAD_PRIORITY_MAIN - 1, 0,
                                thread2_func, NULL, "thread2");
    thread1_pid = thread_create(thread_stack1, sizeof(thread_stack1), THREAD_PRIORITY_MAIN, 0,
                                thread1_func, NULL, "thread1");

    return 0;
}