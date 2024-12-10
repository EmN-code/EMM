#include <xtimer.h>
#include <stdio.h>
#include <msg.h>
#include <event.h>

#define MAIN_QUEUE_SIZE (8)
#define INTERVAL_USEC   (1000000U)  // 1 second in microseconds

static msg_t _msg_queue[MAIN_QUEUE_SIZE];

static void _callback(void *arg) {
    (void)arg;
    printf("Event triggered!\n");
}

int main(void) {
    msg_init_queue(_msg_queue, MAIN_QUEUE_SIZE);

    evtimer_t evtimer;
    evtimer_msg_event_t event;

    evtimer_init_msg(&evtimer);
    evtimer_set(&evtimer.timer, INTERVAL_USEC);

    evtimer_add_msg(&evtimer, &event, thread_getpid());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        _callback(NULL);
    }
    return 0;
}