#include <ztimer.h>              // Including ztimer header
#include <stdio.h>               // Standard I/O operations in C
#include <event.h>               // Event API for message boxes

#define TIMER_INTERVAL (5 * ZTIMER_MSEC)

static evtimer_t my_ev_timer;
static msg_t msg_q[8];

void timer_event_callback(void *arg) {
    (void)arg;
    printf("Timer event triggered!\n");
    msg_t msg;
    msg_send(&msg, thread_getpid());
}

int main(void) {
    msg_init_queue(msg_q, 8);
    evtimer_init(&my_ev_timer, timer_event_callback, NULL);
    ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    while (1) {
        xtimer_usleep(1000000); // Sleep for 1 second
    }
    return 0;
}