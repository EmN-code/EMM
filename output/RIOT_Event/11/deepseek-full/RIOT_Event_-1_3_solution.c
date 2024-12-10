
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stddef.h>
#include <thread.h>
#include <timex.h>
#include <ztimer.h>
#include <xtimer.h>
#include <mbox.h>
#include <evtimer_mbox.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_mbox_t evtimer;
static mbox_t mbox;
static char stack[THREAD_STACKSIZE_DEFAULT];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

void *event_loop(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        periodic_callback(NULL);
    }

    return NULL;
}

int main(void)
{
    kernel_pid_t pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                     THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    mbox_init(&mbox, NULL, 0);
    evtimer_mbox_init(&evtimer, &mbox);

    evtimer_mbox_event_t event;
    event.event.offset = 0;
    event.event.callback = NULL;
    event.event.arg = NULL;
    event.event.next = NULL;
    event.event.prev = NULL;
    event.event.msg.sender_pid = pid;

    while (1) {
        evtimer_mbox_add_event(&evtimer, &event, PERIODIC_INTERVAL);
        xtimer_sleep(1);  // Sleep for 1 second to maintain the periodic interval
    }

    return 0;
}
