#include <ztimer.h>
#include <msg.h>
#include <stdio.h>

#define MAIN_QUEUE_SIZE (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered, sending message.\n");
    msg_t msg;
    msg_send(&msg, sched_active_pid);
}

int main(void) {
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    ztimer_t timer;
    timer.callback = timer_callback;
    timer.arg = NULL;
    timer.target = 0; // This should be modified as per the required periodic time
    timer.long_target = false;
    ztimer_set(ZTIMER_USEC, &timer, 1000000); // Set up a timer for 1 second

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        printf("Message received.\n");
    }

    return 0;
}