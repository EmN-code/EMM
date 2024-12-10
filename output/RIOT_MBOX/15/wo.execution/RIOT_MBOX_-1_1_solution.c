#include <ztimer.h>
#include <stdio.h>
#include <msg.h>
#include <evtimer.h>

#define INTERVAL (ZTIMER_SEC * 1) // 1-second interval
#define MAIN_QUEUE_SIZE     (8)

static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

void event_handler(evtimer_event_t *event) {
    printf("Event occurred!\n");
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_event_t event;

    evtimer.callback = event_handler;
    evtimer.events = &event;

    ztimer_t *timer = &evtimer.timer;
    timer->callback = event_handler;
    timer->value = 0;
    timer->long_interval = 0;

    evtimer_add_mbox(&evtimer, &event, NULL);

    printf("Starting event timer...\n");
    ztimer_set(ZTIMER_MSEC, timer, INTERVAL);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}