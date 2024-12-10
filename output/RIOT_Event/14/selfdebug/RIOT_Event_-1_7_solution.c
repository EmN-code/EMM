#include <ztimer.h>
#include <thread.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * ZTIMER_MSEC_PER_SEC)

static void callback(void *arg) {
    (void) arg;
    puts("Timer callback triggered!");
}

int main(void) {
    msg_t msg;
    msg_init_queue(&msg, 1);

    ztimer_t my_timer;
    ztimer_init();  // No arguments needed here
    ztimer_set_cb(ZTIMER_MSEC, &my_timer, callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
        printf("Periodic event active.\n");
    }
    return 0;
}