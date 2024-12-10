
#include "msg.h"
#include "thread.h"
#include "ztimer.h"
#include "kernel_defines.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdbool.h"

#define MSG_QUEUE_SIZE (8)
#define PERIODIC_INTERVAL (1000000U)  // 1 second in microseconds

static msg_t msg_queue[MSG_QUEUE_SIZE];
static kernel_pid_t periodic_thread_pid;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
    msg_t msg;
    msg.content.value = 0;  // Dummy message content
    msg_send(&msg, periodic_thread_pid);
}

void *periodic_thread(void *arg)
{
    (void)arg;
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    periodic_thread_pid = sched_active_pid;

    while (1) {
        ztimer_sleep(ZTIMER_USEC, PERIODIC_INTERVAL);
        periodic_callback(NULL);
    }

    return NULL;
}

int main(void)
{
    thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        periodic_thread, NULL, "periodic_thread");

    msg_t msg;
    while (1) {
        msg_receive(&msg);
        printf("Message received in main thread!\n");
    }

    return 0;
}
