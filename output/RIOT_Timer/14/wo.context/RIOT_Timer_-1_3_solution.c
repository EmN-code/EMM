#include <ztimer.h>
#include <xtimer.h>
#include "msg.h"

#define INTERVAL_SEC   (1U)

void timer_callback(void *arg) {
    (void)arg;
    // Toggle LED here. This is hardware specific so adjust according to your board
    puts("LED Toggled");
}

int main(void) {
    ztimer_t timer;

    ztimer_init(ZTIMER_MSEC);
    ztimer_set_cb(ZTIMER_MSEC, &timer, INTERVAL_SEC * 1000, timer_callback, NULL);
    
    msg_t msg;
    while (1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, INTERVAL_SEC * 1000);
        // simulate a blocking wait
        msg_receive(&msg);
    }
    return 0;
}