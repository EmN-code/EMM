#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <event/evtimer.h>
#include <thread.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static char event_thread_stack[THREAD_STACKSIZE_DEFAULT];
static msg_t event_queue[8];
static evtimer_t evtimer;
static evtimer_event_t evtimer_events[8];

void *_event_thread(void *args) {
    (void)args;
    msg_init_queue(event_queue, 8);

    evtimer_t evtimer;
    evtimer_event_t events[1];
    evtimer_init(&evtimer, events, 1, NULL);

    evtimer_event_t *event = &events[0];
    evtimer_add_periodic(&evtimer, event, INTERVAL);

    while (1) {
        msg_t msg;
        if (msg_receive(&msg) == 1) {
            puts("Event received!");
        }
    }
    return NULL;
}

int main(void) {
    thread_create(event_thread_stack, sizeof(event_thread_stack),
                  THREAD_PRIORITY_MAIN - 1, 0, _event_thread, NULL, "event");
    return 0;
}