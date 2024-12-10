#include <ztimer.h>
#include <stdio.h>
#include <event.h>

#define INTERVAL (1U * ZTIMER_MSEC_PER_SEC)  // Interval for the periodic event

void callback(void *arg) {
    (void)arg;
    printf("Callback triggered!\n");
}

int main() {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    ztimer_t timer;
    ztimer_init(ZTIMER_MSEC);
    ztimer_set_msg(ZTIMER_MSEC, &timer, INTERVAL, &msg_queue[0], callback);

    while (1) {
        msg_receive(&msg_queue[0]);
        callback(NULL);
        ztimer_periodic_wakeup(ZTIMER_MSEC);
    }

    return 0;
}