#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <evtimer_msg.h>

void event_callback(void *arg) {
    printf("Event triggered!\n");
}

int main(void) {
    evtimer_msg_event_t event;
    evtimer_msg_t evtimer;
    msg_t msg_queue[8];

    msg_init_queue(msg_queue, 8);

    event.event.callback = event_callback;
    event.event.interval = 1000000;  // Set interval to 1 second (1000000 microseconds)

    kernel_pid_t pid = thread_getpid();

    evtimer_msg_init_queue(&evtimer, msg_queue, sizeof(msg_queue) / sizeof(msg_t));
    evtimer_add_msg(&evtimer, &event, pid);

    while(1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}