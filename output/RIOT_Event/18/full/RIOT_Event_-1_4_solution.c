#include "xtimer.h"
#include "stdio.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "event/timeout.h"
#include "msg.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"

#define INTERVAL (1U * US_PER_SEC)  // 1 second interval in microseconds

static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t thread_pid;
evthread_event_t event;

void periodic_event_callback(void *arg)
{
    (void)arg;  // suppress unused warning
    puts("Callback triggered!");
}

void *event_handler_thread(void *arg)
{
    (void)arg;  // suppress unused warning
    while (1) {
        evtimer_mbox_t mbox;
        evtimer_mbox_init(&mbox);
        evtimer_mbox_event_init(&event, periodic_event_callback, NULL);
        evtimer_mbox_add(&evtimer, &event, INTERVAL);
        thread_sleep();  // put the handler to sleep until next event
    }
    return NULL;
}

int main(void)
{
    puts("RIOT Timer Event Example");

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                               event_handler_thread, NULL, "event_handler");

    if (thread_pid <= KERNEL_PID_UNDEF) {
        puts("Failed to create thread");
        return 1;
    }

    msg_t msg;
    while (1) {
        msg_receive(&msg);  // wait for messages, ensuring main keeps running
    }

    return 0;
}
