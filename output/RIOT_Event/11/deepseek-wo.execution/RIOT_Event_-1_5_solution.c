
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <ztimer.h>
#include <evtimer_mbox.h>
#include <msg.h>
#include <mbox.h>

#define STACKSIZE 512
#define MSG_QUEUE_SIZE 8

static char stack[STACKSIZE];
static msg_t msg_queue[MSG_QUEUE_SIZE];
static mbox_t mbox;
static evtimer_t evtimer;

void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, MSG_QUEUE_SIZE);
    mbox_init(&mbox, msg_queue, MSG_QUEUE_SIZE);
    evtimer_init_mbox(&evtimer);

    evtimer_event_t event;
    event.handler = (ev_callback_t)periodic_callback;
    event.arg = NULL;

    xtimer_t timer;
    timer.target = 0;
    timer.long_target = 0;
    xtimer_set(&timer, XTIMER_TICKS(1 * US_PER_SEC));

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        thread_yield();
    }

    return 0;
}
