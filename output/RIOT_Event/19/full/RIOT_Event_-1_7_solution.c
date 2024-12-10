#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "kernel_defines.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "mutex.h"

#define TIMER_MSG_QUEUE_SIZE   (8U)
#define TIMER_INTERVAL        (1U * US_PER_SEC)

static char stack[THREAD_STACKSIZE_MAIN];
msg_t msg_queue[TIMER_MSG_QUEUE_SIZE];
evtimer_t evtimer;

void timer_callback(void *arg)
{
    (void)arg;
    puts("Timer callback triggered.");
}

void *timer_thread(void *arg)
{
    (void)arg;
    evtimer_event_t event;

    msg_init_queue(msg_queue, TIMER_MSG_QUEUE_SIZE);

    evtimer_mbox_init(&evtimer);
    evtimer_mbox_event_init(&event, timer_callback, NULL);
    evtimer_mbox_add(&evtimer, &event, TIMER_INTERVAL);

    evtimer_mbox_run(&evtimer);

    return NULL;
}

int main(void)
{
    puts("Starting event timer example...");

    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  timer_thread, NULL, "timer thread");

    return 0;
}