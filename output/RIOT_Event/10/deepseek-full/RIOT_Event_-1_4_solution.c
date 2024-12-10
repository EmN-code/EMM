
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdatomic.h>
#include <msg.h>
#include <thread.h>
#include <timex.h>
#include <ztimer.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static kernel_pid_t periodic_thread_pid;
static msg_t periodic_msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

void *periodic_thread(void *arg)
{
    (void)arg;
    msg_init_queue(periodic_msg_queue, sizeof(periodic_msg_queue));

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        periodic_callback(NULL);
    }

    return NULL;
}

int main(void)
{
    periodic_thread_pid = thread_create(periodic_msg_queue, sizeof(periodic_msg_queue),
                                        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
                                        periodic_thread, NULL, "periodic_thread");

    while (1) {
        ztimer_sleep(ZTIMER_USEC, PERIODIC_INTERVAL);
        msg_t msg;
        msg.content.ptr = NULL;
        msg_send(&msg, periodic_thread_pid);
    }

    return 0;
}
