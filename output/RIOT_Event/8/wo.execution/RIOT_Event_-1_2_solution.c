#include <ztimer.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <event/thread.h>
#include <event/periodic.h>

void timer_callback(void *arg) {
    /* Timer callback function code */
    printf("Timer callback called\n");
}

int main(void) {
    /* Initialize a ztimer periodical timer */
    static ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, 1000, timer_callback, NULL);

    /* Start the timer */
    while (1) {
        ztimer_periodic_update(&periodic_timer);
    }

    return 0;
}
