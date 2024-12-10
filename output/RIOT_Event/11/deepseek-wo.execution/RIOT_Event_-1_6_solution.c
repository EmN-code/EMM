
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <msg.h>
#include <evtimer_msg.h>

#define STACK_SIZE (1024)
#define PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define PERIODIC_INTERVAL (1 * US_PER_SEC)

static char stack[STACK_SIZE];
static kernel_pid_t pid;
static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_t msg_array[8];
    msg_init_queue(msg_array, 8);

    pid = thread_create(stack, sizeof(stack), PRIORITY, THREAD_CREATE_STACKTEST, periodic_callback, NULL, "periodic_thread");

    evtimer_init_mbox(&evtimer);

    evtimer_msg_event_t event;
    event.event.offset = 0;
    event.event.interval = PERIODIC_INTERVAL;
    event.event.next = NULL;
    event.msg.content.ptr = NULL;
    event.msg.type = 0;
    event.msg.sender_pid = pid;

    evtimer_add_msg(&evtimer, &event, pid);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.content.ptr == NULL) {
            periodic_callback(NULL);
        }
    }

    return 0;
}
