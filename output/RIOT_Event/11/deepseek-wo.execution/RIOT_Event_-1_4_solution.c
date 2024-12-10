
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <msg.h>
#include <thread.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <mbox.h>

#define THREAD_STACKSIZE 512

static char thread_stack[THREAD_STACKSIZE];
static kernel_pid_t thread_pid;
static msg_t msg_queue[16];
static mbox_t mbox;
static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

void *event_thread(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        mbox_get(&mbox, &msg, 1);
        periodic_callback(NULL);
    }

    return NULL;
}

int main(void)
{
    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init_mbox(&evtimer);

    thread_pid = thread_create(thread_stack, sizeof(thread_stack),
                               THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST,
                               event_thread, NULL, "event_thread");

    evtimer_msg_event_t event;
    event.event.handler = (ev_callback_t)periodic_callback;
    event.event.arg = NULL;

    xtimer_t timer;
    timer.target = timer.long_target = xtimer_now() + XTIMER_TICKS(1000000);
    timer.callback = (xtimer_callback_t)evtimer_add_msg;
    timer.arg = &evtimer;

    while (1) {
        xtimer_set(&timer, XTIMER_TICKS(1000000));
        xtimer_usleep(1000000);
    }

    return 0;
}
